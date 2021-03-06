    // Solve the momentum equation

    #include "computeCoriolisForce.H"

    tmp<fvVectorMatrix> UEqn
    (
        // fvm::ddt(U)                             // time derivative
        fvm::div(phi, U)                        // convection
      + turbulence->divDevReff(U)               // momentum flux, blending of LES-RANS bij is done in divDevReff()
      + fvc::div(Rwall)
      - fCoriolis                               // Coriolis force
      // - turbines.force()                        // turbine body forces
      - SourceU                                 // mesoscale source terms
    );

    UEqn().relax();

    // Not constraining U to a fixed value using fvOptions as it's done manually with SourceU
    // fvOptions.constrain(UEqn())

    if (simple.momentumPredictor())
    {
        solve
        (
            UEqn()
            ==
            fvc::reconstruct
            (
                (
                  - fvc::snGrad(p_rgh)          // modified pressure gradient
                  - ghf*fvc::snGrad(rhok)       // buoyancy force
                ) * mesh.magSf()
            )
        );
    }
