/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright held by original author
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
    Foam::isoBubble

Description
    Bubble described using an iso-surface mesh. 

SourceFiles
    isoBubble.C

Authors
    Tomislav Maric tomislav@sourceflux.de 
    Jens Hoepken jens@sourceflux.de
    Kyle Mooney kyle.g.mooney@gmail.com

\*---------------------------------------------------------------------------*/

#ifndef isoBubble_H
#define isoBubble_H

#include "volPointInterpolation.H"
#include "regIOobject.H"
#include "autoPtr.H"
#include "volFieldsFwd.H"
#include "scalarField.H"
#include "pointFields.H"
#include "isoSurfacePoint.H"
#include "fileName.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace bookExamples
{

/*---------------------------------------------------------------------------*\
                         Class isoBubble Declaration
\*---------------------------------------------------------------------------*/

class isoPointFieldCalc
{
    // Computed point iso-field.
    scalarField field_; 

    public: 

        isoPointFieldCalc(const volScalarField& isoField)
        {
            this->calcPointField(isoField);
        }

        virtual void calcPointField(const volScalarField& isoField)
        {
            volPointInterpolation pointInterpolation (isoField.mesh());
            field_ = pointInterpolation.interpolate(isoField)();
        }

        virtual ~isoPointFieldCalc() = default;

        const scalarField& field() const
        {
            return field_;
        }
};

class isoBubble
    :
        public regIOobject
{
    isoPointFieldCalc isoPointField_; 

    // Bubble geometry described with an iso-surface mesh.
    isoSurfacePoint bubbleSurf_; 

    // Number of zeros that are prepended to the timeIndex() 
    // for written VTK files. 
    label timeIndexPad_; 

    // Output format.
    word outputFormat_; 

    fileName paddWithZeros(const fileName& input) const;

public:

    // Constructors

        //- Construct from components
        isoBubble 
        (
            const IOobject& io, 
            const volScalarField& isoField, 
            scalar isoValue = 0.5,
            bool isTime = false,
            label timeIndexPad = 10, 
            word outputFormat = "vtk", 
            bool regularize = true
        );

        // Destructor
        virtual ~isoBubble() = default; 

        virtual bool write (fileName file) const;

        virtual bool write(const bool valid=true) const;
        
        virtual bool writeData(Ostream& os) const; 

        scalar area() const;

        vector center() const;

};

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace bookExamples 


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
