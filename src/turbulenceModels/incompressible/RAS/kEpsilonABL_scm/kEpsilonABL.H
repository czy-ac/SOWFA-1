/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2011-2014 OpenFOAM Foundation
     \\/     M anipulation  |
-------------------------------------------------------------------------------
License
    This file is part of OpenFOAM.
    OpenFOAM is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.
    You should have received a copy of the GNU General Public License
    along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.
Class
    Foam::incompressible::RASModels::kEpsilonABL
Group
    grpIcoRASTurbulence
Description
    k-epsilon turbulence model for atmospheric flows.
    Reference:
    \verbatim
        Launder, B.E., and Spalding, D.B.,
        "Mathematical Models of Turbulence",
        Academic Press, 1972.
        Launder, B.E., and Spalding, D.B.,
        "Computational Methods for Turbulent Flows",
        Comp. Meth in Appl Mech & Eng'g, Vol 3, 1974, pp 269-289.
    \endverbatim
    The default model coefficients correspond to the following:
    \verbatim
        kEpsilonABLCoeffs
        {
            Cmu         0.09;
            C1          1.44;
            C2          1.92;
            sigmaEps    1.3;
        }
    \endverbatim
SourceFiles
    kEpsilonABL.C
\*---------------------------------------------------------------------------*/

#ifndef kEpsilonABL_H
#define kEpsilonABL_H

#include "RASModel.H"
#include "uniformDimensionedFields.H"
#include "IOdictionary.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace incompressible
{
namespace RASModels
{

/*---------------------------------------------------------------------------*\
                           Class kEpsilonABL Declaration
\*---------------------------------------------------------------------------*/

class kEpsilonABL
:
    public RASModel
{

protected:

    // Protected data

        // Model coefficients
            dimensionedScalar Cmu_;
            dimensionedScalar Clambda_;
            dimensionedScalar Ceps1_;
            dimensionedScalar Ceps2_;
            dimensionedScalar sigmak_;
            dimensionedScalar sigmaEps_;
            dimensionedScalar kappa_;
            dimensionedScalar lmax_;
            volScalarField alphaB_;
            volScalarField Ceps1Star_;
            volScalarField Ceps3_;
            volScalarField lm_;



        // Fields
            volScalarField k_;
            volScalarField epsilon_;
            volScalarField nut_;


        // Other necessary information.
            IOdictionary transportDict_;
            word TName_;
            const volScalarField& T_;
            const uniformDimensionedVectorField& g_;
            dimensionedScalar TRef_;
            dimensionedScalar Prt_;
            vector upVec_;


        // LES-RANS bij mixing related parameters
            const volSymmTensorField& bij_;
            dimensionedScalar mix_startTime_;
            dimensionedScalar mix_duration_;
            dimensionedScalar mix_ratio_cap_;
            dimensionedScalar mix_verbose_;


public:

    //- Runtime type information
    TypeName("kEpsilonABL");

    // Constructors

        //- Construct from components
        kEpsilonABL
        (
            const volVectorField& U,
            const surfaceScalarField& phi,
            transportModel& transport,
            const word& turbulenceModelName = turbulenceModel::typeName,
            const word& modelName = typeName
        );


    //- Destructor
    virtual ~kEpsilonABL()
    {}


    // Member Functions
        //- Compute the maximum length scale
        virtual void computeMaxLengthScale();

        //- Return the turbulence viscosity
        virtual tmp<volScalarField> nut() const
        {
            return nut_;
        }

        //- Return the effective diffusivity for k
        tmp<volScalarField> DkEff() const
        {
            return tmp<volScalarField>
            (
                new volScalarField("DkEff", nut_/sigmak_ + nu())
            );
        }

        //- Return the effective diffusivity for epsilon
        tmp<volScalarField> DepsilonEff() const
        {
            return tmp<volScalarField>
            (
                new volScalarField("DepsilonEff", nut_/sigmaEps_ + nu())
            );
        }

        //- Return the turbulence kinetic energy
        virtual tmp<volScalarField> k() const
        {
            return k_;
        }

        //- Return the turbulence kinetic energy dissipation rate
        virtual tmp<volScalarField> epsilon() const
        {
            return epsilon_;
        }

        //- Return the Reynolds stress tensor
        virtual tmp<volSymmTensorField> R() const;

        // //- Return the LES-RANS mixed Reynolds stress tensor
        // virtual tmp<volSymmTensorField> Rmix() const;

        //- Return the effective stress tensor including the laminar stress
        virtual tmp<volSymmTensorField> devReff() const;

        //- Return the source term for the momentum equation
        virtual tmp<fvVectorMatrix> divDevReff(volVectorField& U) const;

        //- Return the source term for the momentum equation
        virtual tmp<fvVectorMatrix> divDevRhoReff
        (
            const volScalarField& rho,
            volVectorField& U
        ) const;

        //- Solve the turbulence equations and correct the turbulence viscosity
        virtual void correct();

        //- Read RASProperties dictionary
        virtual bool read();
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace RASModels
} // End namespace incompressible
} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
