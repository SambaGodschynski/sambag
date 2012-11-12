SET ( SAMBAG_SOURCES
<<<<<<< HEAD
	sambag/com/Common.cpp
	sambag/com/Exception.cpp
	sambag/com/FileHandler.cpp
	sambag/com/FileSystem.cpp
	sambag/cpsqlite/DataBase.cpp
	sambag/cpsqlite/sambag_sqlite.c
=======
	sambag/xml/cpAtl/SimpleTagCreator.cpp
	sambag/xml/cpAtl/MarkupBuilder.cpp
	sambag/disco/IPattern.cpp
	sambag/disco/IDiscoFactory.cpp
	sambag/disco/Coordinate.cpp
>>>>>>> bb6218b6f143019fb0c135a45884539536084728
	sambag/disco/CairoDiscoFactory.cpp
	sambag/disco/Geometry.cpp
	sambag/disco/GeometrySerialization.cpp
	sambag/disco/CairoDrawContext.cpp
	sambag/disco/CairoPattern.cpp
	sambag/disco/ColorRGBA.cpp
<<<<<<< HEAD
	sambag/disco/Coordinate.cpp
	sambag/disco/FileResourceManager.cpp
	sambag/disco/Geometry.cpp
	sambag/disco/GeometrySerialization.cpp
	sambag/disco/IDiscoFactory.cpp
	sambag/disco/IPattern.cpp
	sambag/disco/components/AButton.cpp
	sambag/disco/components/AColumnBrowser.cpp
=======
	sambag/disco/CairoSurface.cpp
	sambag/disco/components/CellRendererPane.cpp
	sambag/disco/components/GridLayout.cpp
	sambag/disco/components/DefaultButtonModell.cpp
	sambag/disco/components/WindowToolkit.cpp
	sambag/disco/components/RedrawManager.cpp
>>>>>>> bb6218b6f143019fb0c135a45884539536084728
	sambag/disco/components/AComponent.cpp
	sambag/disco/components/DefaultSingleSelectionModell.cpp
	sambag/disco/components/RootPane.cpp
	sambag/disco/components/Menu.cpp
	sambag/disco/components/AList.cpp
	sambag/disco/components/AColumnBrowser.cpp
	sambag/disco/components/AButton.cpp
	sambag/disco/components/Window.cpp
	sambag/disco/components/ColumnView.cpp
	sambag/disco/components/AKnob.cpp
	sambag/disco/components/Panel.cpp
	sambag/disco/components/DefaultListCellRenderer.cpp
	sambag/disco/components/BorderLayout.cpp
	sambag/disco/components/Button.cpp
	sambag/disco/components/MessageBox.cpp
	sambag/disco/components/Knob.cpp
	sambag/disco/components/ALayoutManager.cpp
	sambag/disco/components/Scrollbar.cpp
	sambag/disco/components/DefaultBoundedRangeModel.cpp
	sambag/disco/components/IBorder.cpp
	sambag/disco/components/ScrollPane.cpp
	sambag/disco/components/ViewportLayout.cpp
	sambag/disco/components/BoxLayout.cpp
	sambag/disco/components/Label.cpp
	sambag/disco/components/AContainer.cpp
	sambag/disco/components/SizeRequirements.cpp
	sambag/disco/components/ColumnBrowser.cpp
	sambag/disco/components/DefaultListSelectionModel.cpp
	sambag/disco/components/ActionMap.cpp
	sambag/disco/components/MenuSelectionManager.cpp
	sambag/disco/components/Graphics.cpp
	sambag/disco/components/Viewport.cpp
	sambag/disco/components/PopupMenu.cpp
	sambag/disco/components/ButtonGroup.cpp
	sambag/disco/components/List.cpp
	sambag/disco/components/FramedWindow.cpp
	sambag/disco/components/MenuItem.cpp
	sambag/disco/components/Timer.cpp
	sambag/disco/components/ScrollPaneLayout.cpp
	sambag/disco/components/FlowLayout.cpp
	sambag/disco/components/events/ContainerEvent.cpp
	sambag/disco/components/events/ListDataEvent.cpp
	sambag/disco/components/events/HierarchyEvent.cpp
	sambag/disco/components/events/MouseEventCreator.cpp
	sambag/disco/components/events/MouseEvent.cpp
	sambag/disco/components/events/ListSelectionEvent.cpp
	sambag/disco/components/ui/DefaultMenuLayout.cpp
	sambag/disco/components/ui/AComponentUI.cpp
	sambag/disco/components/ui/UIManager.cpp
	sambag/disco/components/ui/ALookAndFeel.cpp
	sambag/disco/components/ui/basic/BasicColumnViewUI.cpp
	sambag/disco/components/ui/basic/BasicArrowButton.cpp
	sambag/disco/components/ui/basic/BasicLabelUI.cpp
	sambag/disco/components/ui/basic/BasicLookAndFeel.cpp
	sambag/disco/components/ui/basic/BasicScrollPaneUI.cpp
	sambag/disco/components/ui/basic/BasicKnobUI.cpp
	sambag/disco/components/windowImpl/Win32WindowImpl.cpp
	sambag/disco/components/windowImpl/X11WindowImpl.cpp
	sambag/disco/components/windowImpl/Win32WindowToolkit.cpp
	sambag/disco/components/windowImpl/Win32MMTimerImpl.cpp
	sambag/disco/components/windowImpl/TestWindowImpl.cpp
	sambag/disco/components/windowImpl/BoostTimerImpl.cpp
	sambag/disco/components/windowImpl/X11WindowToolkit.cpp
	sambag/disco/components/windowImpl/Win32TimerImpl.cpp
	sambag/disco/components/windowImpl/WindowTestToolkit.cpp
	sambag/disco/svg/AttributeParser.cpp
	sambag/disco/svg/Svg.cpp
	sambag/disco/svg/SvgUse.cpp
	sambag/disco/svg/SvgLine.cpp
	sambag/disco/svg/SvgCompound.cpp
	sambag/disco/svg/SvgLinearGradient.cpp
	sambag/disco/svg/HtmlColors.cpp
	sambag/disco/svg/SvgStyle.cpp
	sambag/disco/svg/SvgRadialGradient.cpp
	sambag/disco/svg/SvgText.cpp
	sambag/disco/svg/SvgEllipse.cpp
	sambag/disco/svg/StyleAttributeSetters.cpp
	sambag/disco/svg/StyleParser.cpp
	sambag/disco/svg/SvgPath.cpp
	sambag/disco/svg/SvgBuilder.cpp
	sambag/disco/svg/PathInstructions.cpp
	sambag/disco/svg/SvgGroup.cpp
	sambag/disco/svg/SvgDefs.cpp
	sambag/disco/svg/SvgCircle.cpp
	sambag/disco/svg/SvgRoot.cpp
	sambag/disco/svg/units/Units.cpp
	sambag/disco/svg/graphicElements/Compound.cpp
	sambag/disco/svg/graphicElements/Line.cpp
	sambag/disco/svg/graphicElements/PathHelper.cpp
	sambag/disco/svg/graphicElements/GraphicElement.cpp
	sambag/disco/svg/graphicElements/Image.cpp
	sambag/disco/svg/graphicElements/Text.cpp
	sambag/disco/svg/graphicElements/RefElement.cpp
	sambag/disco/svg/graphicElements/SceneGraphHelper.cpp
	sambag/disco/svg/graphicElements/SceneGraph.cpp
	sambag/disco/svg/graphicElements/PointContainer.cpp
	sambag/disco/svg/graphicElements/Path.cpp
	sambag/disco/svg/graphicElements/Arc.cpp
	sambag/disco/svg/graphicElements/Style.cpp
	sambag/disco/svg/graphicElements/Rect.cpp
	sambag/cpsqlite/sambag_sqlite.c
	sambag/cpsqlite/DataBase.cpp
	sambag/math/Matrix.cpp
	sambag/com/Exception.cpp
	sambag/com/FileSystem.cpp
	sambag/com/FileHandler.cpp
	sambag/com/Common.cpp
)
SET ( SAMBAG_TESTSOURCES
<<<<<<< HEAD
	sambag/com/tests/FileSystemTest.cpp
	sambag/com/tests/TestArithmeticWrapper.cpp
	sambag/com/tests/TestHelper.cpp
	sambag/com/tests/TestPropertyChanged.cpp
	sambag/com/tests/TestSharedOrWeak.cpp
	sambag/com/tests/TestThread.cpp
	sambag/cpsqlite/tests/DBTest.cpp
	sambag/disco/components/tests/TestAComponent.cpp
	sambag/disco/components/tests/TestAContainer.cpp
=======
	sambag/xml/tests/XML2ObjTest.cpp
	sambag/xml/tests/TestMarkupBuilder.cpp
	sambag/disco/components/tests/TestConcreteComponents.cpp
>>>>>>> bb6218b6f143019fb0c135a45884539536084728
	sambag/disco/components/tests/TestBoostTimerImpl.cpp
	sambag/disco/components/tests/TestComponents.cpp
	sambag/disco/components/tests/TestRootPane.cpp
	sambag/disco/components/tests/TestAContainer.cpp
	sambag/disco/components/tests/TestFlowLayout.cpp
	sambag/disco/components/tests/TestDefaultTreeModel.cpp
	sambag/disco/components/tests/TestLookAndFeel.cpp
	sambag/disco/components/tests/TestWindow.cpp
	sambag/disco/components/tests/TestMouseEvent.cpp
	sambag/disco/components/tests/TestList.cpp
	sambag/disco/components/tests/TestPopupMenu.cpp
	sambag/disco/components/tests/TestUIManager.cpp
	sambag/disco/components/tests/TestAComponent.cpp
	sambag/disco/tests/TestCairoDrawContext.cpp
<<<<<<< HEAD
	sambag/disco/tests/TestFileResourceManager.cpp
	sambag/disco/tests/TestGeometrics.cpp
	sambag/disco/tests/TestGeometrySerialization.cpp
	sambag/disco/tests/TestPattern.cpp
=======
>>>>>>> bb6218b6f143019fb0c135a45884539536084728
	sambag/disco/tests/TestSceneGraph.cpp
	sambag/disco/tests/HelperForTesting.cpp
	sambag/disco/tests/TestGeometrySerialization.cpp
	sambag/disco/tests/TestGeometrics.cpp
	sambag/disco/tests/TestAttributeParser.cpp
	sambag/disco/tests/TestSvg.cpp
	sambag/disco/tests/TestPattern.cpp
	sambag/lua/tests/TestLuaHelper.cpp
	sambag/lua/tests/TestLuaRegisterFunction.cpp
	sambag/cpsqlite/tests/DBTest.cpp
	sambag/math/tests/VectorNTest.cpp
	sambag/com/tests/TestSharedOrWeak.cpp
	sambag/com/tests/FileSystemTest.cpp
	sambag/com/tests/TestPropertyChanged.cpp
	sambag/com/tests/TestHelper.cpp
	sambag/com/tests/TestArithmeticWrapper.cpp
	sambag/com/tests/TestThread.cpp
)
add_library(sambag ${SAMBAG_SOURCES})

SET ( SAMBAG_TESTAPPSOURCES
	testApps/componentTests/TestApp/src/DiscoView.cpp
)
#unit tests
SET( UNIT_TESTSOURCE testApps/sambag_tests.cpp ${SAMBAG_TESTSOURCES})
add_executable(unit_tests ${UNIT_TESTSOURCE})
target_link_libraries (unit_tests sambag ${SAMBAG_CLIBS})
#testapp
add_executable(testApp ${SAMBAG_TESTAPPSOURCES})
target_link_libraries (testApp sambag ${SAMBAG_CLIBS})


