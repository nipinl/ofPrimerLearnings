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

Authors
    Tomislav Maric tomislav@sourceflux.de 
    Jens Hoepken jens@sourceflux.de
    Kyle Mooney kyle.g.mooney@gmail.com

\*---------------------------------------------------------------------------*/

#include "isoBubble.H"
#include "error.H"
#include <iostream>
#include <iomanip>
#include <sstream>

namespace Foam {

namespace bookExamples {

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //
    
fileName isoBubble::paddWithZeros(const fileName& input) const
{
    word extension = input.ext(); 

    if (extension.empty())
    {
        extension = outputFormat_; 
    }

    // Get the time index. 
    const Time& runTime = time(); 
    // Get the time index. 
    label timeIndex = runTime.timeIndex(); 

    // Remove extension.
    fileName output = input.lessExt();  

    // Padd the string using stringstream and iomanip.
    std::stringstream ss;
    ss << output << "-" << std::setw(timeIndexPad_) << std::setfill('0') 
        << timeIndex << "." << extension;

    return ss.str(); 
}

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

isoBubble::isoBubble
(
    const IOobject& io, 
    const volScalarField& isoField, 
    scalar isoValue,
    bool isTime , 
    label timeIndexPad,
    word outputFormat, 
    bool regularize
)
    : 
        regIOobject(io, isTime), 
        isoPointField_(isoField), 
        bubbleSurf_(isoField, isoPointField_.field(), isoValue),
        timeIndexPad_(timeIndexPad), 
        outputFormat_(outputFormat)
{}

// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

bool isoBubble::write(fileName file) const
{
    fileName fullPath = file.lessExt(); 
    mkDir(fullPath); 

    fullPath = fullPath + "/" + paddWithZeros(file); 

    bubbleSurf_.write(fullPath); 

    return true;
}

bool isoBubble::write(const bool valid) const
{
    // Create the path to the instance directory.
    mkDir(path());

    fileName outFileName = paddWithZeros(objectPath());

    if (OFstream::debug)
    {
        Info<< "regIOobject::write() : "
            << "writing file " << outFileName << endl; 
    }

    bubbleSurf_.write(outFileName);

    return true;
}

bool isoBubble::writeData(Ostream& os) const
{
    os << bubbleSurf_;
    return true;
}

scalar isoBubble::area() const
{
    scalar area = 0;

    const pointField& points = bubbleSurf_.points(); 
    const List<face>& faces = bubbleSurf_.localFaces(); 

    forAll (faces, I)
    {
        area += mag(faces[I].areaNormal(points));
    }
    cout<<"Calculating area ..."<<endl;
    return area;
}

vector isoBubble::center() const
{
    vector bubbleCenter (0,0,0);

    const pointField& points = bubbleSurf_.points(); 
    forAll(points, I)
    {
        bubbleCenter += points[I];
    }

    return bubbleCenter / bubbleSurf_.nPoints();
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace bookExamples 
} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //


// ************************************************************************* //
