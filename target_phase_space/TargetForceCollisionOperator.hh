#ifndef GEP_TARGET_FORCE_COLLISION_OPERATOR_HH
#define GEP_TARGET_FORCE_COLLISION_OPERATOR_HH

// Adapted from the Geant4 extended/biasing/GB02 example.

#include "G4VBiasingOperator.hh"

#include <map>

class G4BOptrForceCollision;
class G4ParticleDefinition;

class TargetForceCollisionOperator final : public G4VBiasingOperator {
 public:
  explicit TargetForceCollisionOperator(const G4String& name);
  ~TargetForceCollisionOperator() override;

  void AddParticle(const G4String& particleName);
  void StartTracking(const G4Track* track) override;

 private:
  G4VBiasingOperation* ProposeNonPhysicsBiasingOperation(
      const G4Track*, const G4BiasingProcessInterface*) override;
  G4VBiasingOperation* ProposeOccurenceBiasingOperation(
      const G4Track*, const G4BiasingProcessInterface*) override;
  G4VBiasingOperation* ProposeFinalStateBiasingOperation(
      const G4Track*, const G4BiasingProcessInterface*) override;
  void OperationApplied(const G4BiasingProcessInterface*, G4BiasingAppliedCase,
                        G4VBiasingOperation*, const G4VParticleChange*) override;
  void OperationApplied(const G4BiasingProcessInterface*, G4BiasingAppliedCase,
                        G4VBiasingOperation*, G4double, G4VBiasingOperation*,
                        const G4VParticleChange*) override;
  void ExitBiasing(const G4Track*, const G4BiasingProcessInterface*) override;

  std::map<const G4ParticleDefinition*, G4BOptrForceCollision*> operators_;
  G4BOptrForceCollision* currentOperator_ = nullptr;
};

#endif
