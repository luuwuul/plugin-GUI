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

#ifndef __EDITORVIEWPORT_H_80260F3F__
#define __EDITORVIEWPORT_H_80260F3F__

#include "../../JuceLibraryCode/JuceHeader.h"
#include "../Processors/Editors/GenericEditor.h"
#include "../Processors/Merger/MergerEditor.h"
#include "../Processors/ProcessorGraph/ProcessorGraph.h"
#include "../Processors/Splitter/Splitter.h"
#include "../Processors/Splitter/SplitterEditor.h"

#include "../Processors/PluginManager/OpenEphysPlugin.h"

#include "ControlPanel.h"
#include "DataViewport.h"
#include "UIComponent.h"

class GenericEditor;
class SignalChainTabButton;
class SignalChainTabComponent;
class SignalChainScrollButton;
class ControlPanel;
class UIComponent;
class AddProcessor;

/**

  Allows the user to view and edit the signal chain.

  The EditorViewport is one of the most important classes in the GUI application.
  Dragging processors from the ProcessorList into the EditorViewport adds them to the signal chain. The
  newly added processors appear an editors in the EditorViewport. Deleting the editor from the
  EditorViewport removes its associated processor from the signal chain. Moving an editor (by dragging
  and dropping within the EditorViewport) rearranges the order of processing.

  The EditorViewport can be used to browse through multiple parallel signal chains
  (by clicking the buttons on the far left), or to navigate around branching
  signal chains.

  @see UIComponent, ProcessorGraph

*/

class EditorViewport : public Component,
                       public DragAndDropTarget,
                       public Label::Listener
{
public:
    /** Constructor. Adds the buttons for browsing through the signal chains.*/
    EditorViewport (SignalChainTabComponent*);

    /** Destructor. */
    ~EditorViewport();

    /** Highlights the given editor. */
    void highlightEditor (GenericEditor* editor);

    /** Ensures that the user can see the requested editor & its visualizer (if any). */
    void makeEditorVisible (GenericEditor* editor, bool updateSettings = false);

    /** Updates the boundaries and visibility of all the editors in the signal chain. */
    void refreshEditors();

    /** Removes all processors from the signal chain(s).*/
    void clearSignalChain();

    /** Determines whether or not the EditorViewport should respond to
    the component that is currently being dragged. */
    bool isInterestedInDragSource (const SourceDetails& dragSourceDetails);

    /** Called when a dragged item (usually a name from the ProcessorList) enters the
       boundaries of the EditorViewport. Causes the background of the EditorViewport to change colour.*/
    void itemDragEnter (const SourceDetails& dragSourceDetails);

    /** Called when a dragged item (usually a name from the ProcessorList) moves within the
       boundaries of the EditorViewport. Causes existing editors (if any) to shift their position
       to make room for the new processor that could be dropped.*/
    void itemDragMove (const SourceDetails& dragSourceDetails);

    /** Called when a dragged item (usually a name from the ProcessorList) leaves the
       boundaries of the EditorViewport. Causes the background of the EditorViewport to change colour.*/
    void itemDragExit (const SourceDetails& dragSourceDetails);

    /** Called when a dragged item (usually a name from the ProcessorList) is released within the
       boundaries of the EditorViewport. Adds the dropped processor to the signal chain.*/
    void itemDropped (const SourceDetails& dragSourceDetails);

    /** Called when a mouse click begins within the EditorViewport. Usually used to select editors.*/
    void mouseDown (const MouseEvent& e);

    /** Called when a mouse drag occurs within the EditorViewport. Usually used to move editors around in the signal chain.*/
    void mouseDrag (const MouseEvent& e);

    /** Called when a mouse click ends within the EditorViewport. Usually used to indicate that a moving editor has been dropped.*/
    void mouseUp (const MouseEvent& e);

    /** Called when the mouse leaves the boundaries of the EditorViewport.*/
    void mouseExit (const MouseEvent& e);

    /** Called when a key is pressed an the EditorViewport has keyboard focus.*/
    bool keyPressed (const KeyPress& key);

    /** Changes which editor is selected, depending on the keypress (and modifier keys).*/
    void moveSelection (const KeyPress& key);

    /** Called when a label is changed.*/
    void labelTextChanged (Label* label);

    /** Save the current configuration as an XML file. */
    const String saveState (File filename, String* xmlText = nullptr);

    /** Save the current configuration as an XML file. Reference wrapper*/
    const String saveState (File filename, String& xmlText);

    /** Saves the viewport-specific settings (e.g. processor order) */
    void saveEditorViewportSettingsToXml (XmlElement* xml);

    /** Save the current configuration as an XML file. Reference wrapper*/
    std::unique_ptr<XmlElement> createSettingsXml();

    /** Load a saved configuration from an XML file. */
    const String loadState (File filename);

    /** Load a saved configuration from an XML object*/
    const String loadStateFromXml (XmlElement* xml);

    /** Loads the viewport-specific settings (e.g. processor order) */
    void loadEditorViewportSettingsFromXml (XmlElement* xml);

    /** Load a saved plugin configuration from an XML file. */
    const String loadPluginState (File filename, GenericEditor* selectedEditor = nullptr);

    /** Load a saved plugin configuration from an XML file. */
    const String savePluginState (File filename, GenericEditor* selectedEditor = nullptr);

    /** Converts information about a given editor to XML. */
    XmlElement* createNodeXml (GenericProcessor*, bool isStartOfSignalChain);

    /** Sets the parameters of a given processor via XML save files*/
    void setParametersByXML (GenericProcessor*, XmlElement*);

    /** Returns a boolean indicating whether or not the signal chain is empty. */
    bool isSignalChainEmpty();

    /** Returns a boolean indicating whether or not the signal chain is locked */
    bool isSignalChainLocked() { return signalChainIsLocked; }

    /** Determines whether the signal chain can be edited */
    void lockSignalChain (bool shouldLock);

    /** Updates visible editors (called after Processor Graph modifications)*/
    void updateVisibleEditors (Array<GenericEditor*> visibleEditors,
                               int numberOfTabs = 1,
                               int selectedTab = 0);

    /** Removes an editor from the editor array after it's processor has been deleted*/
    void removeEditor (GenericEditor* editor);

    /** Adds a processor to the signal chain, based on a Plugin description */
    GenericProcessor* addProcessor (Plugin::Description desc, int insertionPt);

    /** Deletes all processors that are currently selected */
    void deleteSelectedProcessors();

    /** Adds the parameters for the selected editors to the copy buffer */
    void copySelectedEditors();

    /** Creates a new processor based on XML settings */
    GenericProcessor* createProcessorAtInsertionPoint (XmlElement* processor, int insertionPt, bool ignoreNodeId);

    /** Returns a plugin description from XML settings */
    Plugin::Description getDescriptionFromXml (XmlElement* settings, bool ignoreNodeId);

    /** Changes the I/O path of a spliter or merger*/
    void switchIO (GenericProcessor* processor, int path);

    File currentFile;

    /** Flag to check whether config is being loaded */
    bool loadingConfig;

    /** Draws the EditorViewport background */
    void paint (Graphics& g);

    /** Returns the width of the viewport (for scrolling purposes) */
    int getDesiredWidth();

    /** */
    void copy (Array<XmlElement*>);

    void paste();

    bool editorIsSelected();

    bool canPaste();

    Array<GenericEditor*> editorArray;

    bool somethingIsBeingDraggedOver;

private:
    String message;

    GenericEditor* lastEditor;
    GenericEditor* lastEditorClicked;
    GenericEditor* editorToUpdate;

    int selectionIndex;

    FontOptions font;
    Image sourceDropImage;

    int insertionPoint;
    bool componentWantsToMove;
    int indexOfMovingComponent;

    // Keeps track of the processor type being dragged
    Plugin::Processor::Type dragProcType;

    SignalChainTabComponent* signalChainTabComponent;

    bool shiftDown;

    OwnedArray<XmlElement> copyBuffer;

    Label editorNamingLabel;

    bool signalChainIsLocked = false;

    OwnedArray<AddProcessor> orphanedActions;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EditorViewport);
};

/**

  Clicking the tab button makes the editors for its signal chain visible.

  @see EditorViewport

*/

class SignalChainTabButton : public Button
{
public:
    /** Constructor */
    SignalChainTabButton (int index);

    /** Destructor */
    ~SignalChainTabButton() {}

    int offset;

private:
    /** Draws the SignalChainTabButton.*/
    void paintButton (Graphics& g, bool isMouseOver, bool isButtonDown);

    int num;

    FontOptions buttonFont;
};

/**

  Allows the user to scroll through signal chains when
  there are more than can fit within the boundaries of the
  EditorViewport.

  @see EditorViewport.

*/

class SignalChainScrollButton : public TextButton
{
public:
    /** Constructor*/
    SignalChainScrollButton (int type);

    /** Destructor*/
    ~SignalChainScrollButton() {}

    /** Sets whether the button can be used*/
    void setActive (bool);

    /** Renders the button*/
    void paintButton (Graphics& g, bool isMouseOverButton, bool isButtonDown) override;

    enum type
    {
        UP,
        DOWN
    };

private:
    bool isActive;
    Path path; // inactive, activeNormal, activeOver, activeDown;
};

/**

  Allows the user to navigate between multiple parallel signal chains.

  Each SignalChainTabButton sits on the left-hand side of the EditorViewport
  and is associated with a given signal chain. Clicking the tab button makes
  the editors for its signal chain visible.

  @see EditorViewport

*/

class SignalChainTabComponent : public Component,
                                public Button::Listener
{
public:
    SignalChainTabComponent();
    ~SignalChainTabComponent();

    /** Updates the boundaries and visibility of all the tabs in the signal chain. */
    void refreshTabs (int, int, bool internal = false);

    void resized();

    /** Draws the background of the EditorViewport. */
    void paint (Graphics& g) override;

    /** Draws the outline of the EditorViewport. */
    void paintOverChildren (Graphics& g) override;

    /** Called when one of the buttons the EditorViewport listens to has been clicked.*/
    void buttonClicked (Button* button);

    int offset;

    void setEditorViewport (EditorViewport*);

    Viewport* getViewport() { return viewport.get(); }

    enum directions
    {
        UP,
        DOWN
    };

    int getScrollOffset();
    void setScrollOffset (int offset);

    // Returns the currently selected tab
    int getSelectedTab() { return selectedTab; }

private:
    int numberOfTabs = 0;
    int selectedTab = -1;
    int topTab;

    OwnedArray<SignalChainTabButton> signalChainTabButtonArray;

    std::unique_ptr<SignalChainScrollButton> upButton;
    std::unique_ptr<SignalChainScrollButton> downButton;

    std::unique_ptr<Viewport> viewport;
    EditorViewport* editorViewport;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SignalChainTabComponent);
};

#endif // __EDITORVIEWPORT_H_80260F3F__
