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

#ifndef __PROCESSORLIST_H_C3A661E9__
#define __PROCESSORLIST_H_C3A661E9__

#include "../../JuceLibraryCode/JuceHeader.h"
#include "../AccessClass.h"
#include "../Processors/PluginManager/OpenEphysPlugin.h"
#include "CustomArrowButton.h"

class ProcessorListItem;
class UIComponent;

/**

  Holds a list of processors that can be used to build the signal
  chain.

  The signal chain is created by dragging names of processors from the ProcessorList
  and dropping them in the appropriate order on the EditorViewport.

  The ProcessorList be manually updated every time a new processor is created,
  and the names must match those recognized by the ProcessorGraph.

  @see EditorViewport, ProcessorGraph

*/

class ProcessorList : public Component,
                      public DragAndDropContainer,
                      public ChangeListener,
                      public Timer

{
public:
    /** Constructor**/
    ProcessorList (Viewport* v);

    /** Destructor*/
    ~ProcessorList() {}

    /** Switches the open/closed state of the ProcessorList.*/
    void toggleState();

    /** Called when the user requests a colour change using a ColourSelector.*/
    void changeListenerCallback (ChangeBroadcaster* source);

    /** Returns the open/closed state of the ProcessorList.*/
    bool isOpen();

    /** Draws the ProcessorList. */
    void paint (Graphics& g);

    /** Gets the colours of the different types of processors.*/
    Array<Colour> getColours();

    /** Sets the colours of the different types of processors.*/
    void setColours (Array<Colour>);

    /** Saves the ProcessorList state. */
    void saveStateToXml (XmlElement*);

    /** Loads the ProcessorList state. */
    void loadStateFromXml (XmlElement*);

    /** Fill item list **/
    void fillItemList();

    /** Get list of processors **/
    Array<String> getItemList();

    Plugin::Description getItemDescriptionfromList (const String& processorName);

    /** Set component bounds */
    void resized();

    /** Used to animate list item location */
    void timerCallback();

    /** Returns the height requested by the ProcessorList. Determines whether or not
    to draw scroll bars.*/
    int getTotalHeight();

    /** Update Search button colour*/
    void lookAndFeelChanged() override;

private:
    /** The main method for drawing the ProcessorList.*/
    void drawItems (Graphics& g);

    /** Draws a single item within the ProcessorList.*/
    void drawItem (Graphics& g, ProcessorListItem*);

    /** Draws the name of a single item within the ProcessorList.*/
    void drawItemName (Graphics& g, ProcessorListItem*);

    /** Returns the ProcessorListItem that sits at a given y coordinate.*/
    ProcessorListItem* getListItemForYPos (int y);

    /** Sets the appropriate viewport for drawing.*/
    void setViewport (Graphics& g, bool);

    int currentColour;

    /** Deselects all items within the ProcessorList.*/
    void clearSelectionState();

    bool isDragging;
    int totalHeight, itemHeight, subItemHeight;
    int xBuffer, yBuffer;

    String category;

    /** Called when the mouse moves within the boundaries of the ProcessorList.*/
    void mouseMove (const MouseEvent& e);

    /** Called when the mouse exits the boundaries of the ProcessorList.*/
    void mouseExit (const MouseEvent& e);

    /** Called when a mouse click begins within the boundaries of the ProcessorList.*/
    void mouseDown (const MouseEvent& e);

    /** Called when a mouse drag occurs within the boundaries of the ProcessorList.*/
    void mouseDrag (const MouseEvent& e);

    /** The base item in the list.*/
    std::unique_ptr<ProcessorListItem> baseItem;

    FontOptions listFontLight;
    FontOptions listFontPlain;

    ProcessorListItem* hoverItem;
    int maximumNameOffset;

    Viewport* viewport;

    std::unique_ptr<CustomArrowButton> arrowButton;
    Path openArrowPath;
    Path closedArrowPath;

    std::unique_ptr<ShapeButton> searchButton;
    std::unique_ptr<TextEditor> searchField;
    String searchText;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ProcessorList);
};

/**

  An item within the ProcessorList.

  If a ProcessorListItem has sub-items, it acts as a button that can
  be used to show/hide segments of the ProcessorList. If it has no
  sub-items, then it holds the name of a processor which can be dragged
  onto the EditorViewport to construct the signal chain.

  @see ProcessorList

*/

class ProcessorListItem : public Component
{
public:
    /** Constructor*/
    ProcessorListItem (const String& name,
                       int processorId = -1,
                       Plugin::Type type = Plugin::INVALID,
                       Plugin::Processor::Type processorType = Plugin::Processor::INVALID);

    /** Destructor*/
    ~ProcessorListItem() {}

    /** Returns the number of sub-items for a given ProcessorListItem. */
    int getNumSubItems();

    /** Returns the sub-item for a given index. */
    ProcessorListItem* getSubItem (int index);

    /** Clears all the sub-items owned by the ProcessorListItem. */
    void clearSubItems();

    /** Adds a sub-item. */
    void addSubItem (ProcessorListItem* newItem);

    /** Removes a sub-item with a given index. */
    void removeSubItem (int index);

    /** Returns true if a ProcessorListItem has sub-items, false otherwise. */
    bool hasSubItems();

    /** Removes true if a ProcessorListItem with sub-items is open, false otherwise. */
    bool isOpen();

    /** Sets the open/closed state of a ProcessorListItem with sub-items. */
    void setOpen (bool);

    /** Toggles the open/closed state of a ProcessorListItem with sub-items. */
    void reverseOpenState()
    {
        open = ! open;
    }

    /** Returns true if a ProcessorListItem has been selected by the user, false otherwise. */
    bool isSelected()
    {
        return selected;
    }

    /** Sets selection state of a ProcessorListItem. */
    void setSelected (bool b)
    {
        selected = b;
    }

    /** Returns the name of a ProcessorListItem. */
    const String& getName();

    /** Returns the name of the parent of a ProcessorListItem. */
    const String& getParentName();

    /** Sets the name of the parent of a ProcessorListItem. */
    void setParentName (const String& name);

    /** Determines the colour of the ProcessorListItem (based on enumerator defined in setParentName() method). */
    int colourId;

    const int index;

    const Plugin::Type pluginType;

    const Plugin::Processor::Type processorType;

private:
    bool selected;
    bool open;
    const String name;
    String parentName;

    /** An array of all the sub-items (if any) that belong to this ProcessorListItem. */
    OwnedArray<ProcessorListItem> subItems;
};

#endif // __PROCESSORLIST_H_C3A661E9__
