/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2011-2017 OpenFOAM Foundation
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
    Foam::recirculationControlFvPatchField

Group
    grpGenericBoundaryConditions

Description
    This boundary condition applies a zero-gradient condition from the patch
    internal field onto the patch faces.

Usage
    Example of the boundary condition specification:
    \verbatim
    <patchName>
    {
        type            recirculationControl;
    }
    \endverbatim

SourceFiles
    recirculationControlFvPatchField.C

\*---------------------------------------------------------------------------*/

#ifndef recirculationControlFvPatchField_H
#define recirculationControlFvPatchField_H

#include "fvPatchField.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                 Class recirculationControlFvPatchField Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class recirculationControlFvPatchField
:
    public fvPatchField<Type>
{

public:

    //- Runtime type information
    TypeName("recirculationControl");


    // Constructors

        //- Construct from patch and internal field
        recirculationControlFvPatchField
        (
            const fvPatch&,
            const DimensionedField<Type, volMesh>&
        );

        //- Construct from patch, internal field and dictionary
        recirculationControlFvPatchField
        (
            const fvPatch&,
            const DimensionedField<Type, volMesh>&,
            const dictionary&
        );

        //- Construct by mapping the given recirculationControlFvPatchField<Type>
        //  onto a new patch
        recirculationControlFvPatchField
        (
            const recirculationControlFvPatchField<Type>&,
            const fvPatch&,
            const DimensionedField<Type, volMesh>&,
            const fvPatchFieldMapper&
        );

        //- Construct as copy
        recirculationControlFvPatchField
        (
            const recirculationControlFvPatchField<Type>&
        );

        //- Construct and return a clone
        virtual tmp<fvPatchField<Type>> clone() const
        {
            return tmp<fvPatchField<Type>>
            (
                new recirculationControlFvPatchField<Type>(*this)
            );
        }

        //- Construct as copy setting internal field reference
        recirculationControlFvPatchField
        (
            const recirculationControlFvPatchField<Type>&,
            const DimensionedField<Type, volMesh>&
        );

        //- Construct and return a clone setting internal field reference
        virtual tmp<fvPatchField<Type>> clone
        (
            const DimensionedField<Type, volMesh>& iF
        ) const
        {
            return tmp<fvPatchField<Type>>
            (
                new recirculationControlFvPatchField<Type>(*this, iF)
            );
        }


    // Member functions

        // Evaluation functions

            //- Return gradient at boundary
            virtual tmp<Field<Type>> snGrad() const
            {
                return tmp<Field<Type>>
                (
                    new Field<Type>(this->size(), Zero)
                );
            }

            //- Evaluate the patch field
            virtual void evaluate
            (
                const Pstream::commsTypes commsType =
                    Pstream::commsTypes::blocking
            );

            //- Return the matrix diagonal coefficients corresponding to the
            //  evaluation of the value of this patchField with given weights
            virtual tmp<Field<Type>> valueInternalCoeffs
            (
                const tmp<scalarField>&
            ) const;

            //- Return the matrix source coefficients corresponding to the
            //  evaluation of the value of this patchField with given weights
            virtual tmp<Field<Type>> valueBoundaryCoeffs
            (
                const tmp<scalarField>&
            ) const;

            //- Return the matrix diagonal coefficients corresponding to the
            //  evaluation of the gradient of this patchField
            virtual tmp<Field<Type>> gradientInternalCoeffs() const;

            //- Return the matrix source coefficients corresponding to the
            //  evaluation of the gradient of this patchField
            virtual tmp<Field<Type>> gradientBoundaryCoeffs() const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
    #include "recirculationControlFvPatchField.C"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
