/*
    ------------------------------------------------------------------

    This file is part of the Open Ephys GUI
    Copyright (C) 2024 Open Ephys

    ------------------------------------------------------------------

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

#include "PlaceholderProcessorEditor.h"

PlaceholderProcessorEditor::PlaceholderProcessorEditor (GenericProcessor* parentNode, String pName, String lName, String lVer)
    : GenericEditor (parentNode), processorName (pName), libName (lName), libVersion (lVer)
{
    notfoundLabel = new Label ("Not found", "Plugin not found");
    notfoundLabel->setBounds (10, 28, 170, 15);
    addAndMakeVisible (notfoundLabel);

    libLabel = new Label ("Plugin", libName);
    libLabel->setBounds (10, 45, 170, 40);
    addAndMakeVisible (libLabel);

    nameLabel = new Label ("Processor", "Missing processor: " + processorName);
    nameLabel->setBounds (10, 85, 170, 40);
    addAndMakeVisible (nameLabel);

    desiredWidth = 180;
}

PlaceholderProcessorEditor::~PlaceholderProcessorEditor()
{
}
