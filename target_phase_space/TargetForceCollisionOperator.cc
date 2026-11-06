#include "TargetForceCollisionOperator.hh"

#include "G4BOptrForceCollision.hh"
#include "G4BiasingProcessInterface.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTable.hh"

TargetForceCollisionOperator::TargetForceCollisionOperator(const G4String& name)
    : G4VBiasingOperator(name) {}

TargetForceCollisionOperator::~TargetForceCollisionOperator() {
  for (auto& entry : operators_) delete entry.second;
}

void TargetForceCollisionOperator::AddParticle(const G4String& particleName) {
  const auto* particle =
      G4ParticleTable::GetParticleTable()->FindParticle(particleName);
  if (!particle) {
    G4ExceptionDescription message;
    message << "Particle " << particleName << " was not found";
    G4Exception("TargetForceCollisionOperator::AddParticle",
                "GEPTARGETBIAS001", FatalException, message);
  }
  operators_[particle] = new G4BOptrForceCollision(
      particleName, GetName() + "For" + particleName);
}

void TargetForceCollisionOperator::StartTracking(const G4Track* track) {
  // Force only the incident beam electron. Recursively forcing nuclear
  // daughters causes an exponential track population and invalid statistics.
  const G4bool primaryElectron =
      track->GetParticleDefinition()->GetPDGEncoding() == 11 &&
      track->GetParentID() == 0;
  if (!primaryElectron) {
    currentOperator_ = nullptr;
    return;
  }
  const auto found = operators_.find(track->GetParticleDefinition());
  currentOperator_ = found == operators_.end() ? nullptr : found->second;
}

G4VBiasingOperation*
TargetForceCollisionOperator::ProposeOccurenceBiasingOperation(
    const G4Track* track, const G4BiasingProcessInterface* process) {
  return currentOperator_
             ? currentOperator_->GetProposedOccurenceBiasingOperation(track,
                                                                       process)
             : nullptr;
}

G4VBiasingOperation*
TargetForceCollisionOperator::ProposeNonPhysicsBiasingOperation(
    const G4Track* track, const G4BiasingProcessInterface* process) {
  return currentOperator_
             ? currentOperator_->GetProposedNonPhysicsBiasingOperation(track,
                                                                        process)
             : nullptr;
}

G4VBiasingOperation*
TargetForceCollisionOperator::ProposeFinalStateBiasingOperation(
    const G4Track* track, const G4BiasingProcessInterface* process) {
  return currentOperator_
             ? currentOperator_->GetProposedFinalStateBiasingOperation(track,
                                                                        process)
             : nullptr;
}

void TargetForceCollisionOperator::OperationApplied(
    const G4BiasingProcessInterface* process, G4BiasingAppliedCase biasCase,
    G4VBiasingOperation* operation, const G4VParticleChange* change) {
  if (currentOperator_) {
    currentOperator_->ReportOperationApplied(process, biasCase, operation,
                                              change);
  }
}

void TargetForceCollisionOperator::OperationApplied(
    const G4BiasingProcessInterface* process, G4BiasingAppliedCase biasCase,
    G4VBiasingOperation* occurrence, G4double weight,
    G4VBiasingOperation* finalState, const G4VParticleChange* change) {
  if (currentOperator_) {
    currentOperator_->ReportOperationApplied(process, biasCase, occurrence,
                                              weight, finalState, change);
  }
}

void TargetForceCollisionOperator::ExitBiasing(
    const G4Track* track, const G4BiasingProcessInterface* process) {
  if (currentOperator_) currentOperator_->ExitingBiasing(track, process);
}
