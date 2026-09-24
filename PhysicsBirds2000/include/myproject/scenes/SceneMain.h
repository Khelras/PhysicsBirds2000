/***********************************************************************
Bachelor of Software Engineering
Media Design School
Auckland
New Zealand
(c) 2026 Media Design School
File Name   : SceneMain.h
Description : Declares the SceneMain Class Functions and Properties.
Author      : Angelo Joseph Arawiran Bohol
Mail        : angelo.bohol@mds.ac.nz
**************************************************************************/

#pragma once
#include "Scene.h"

/// <summary>
///     The main scene which derives from the base scene class.
/// </summary>
class SceneMain : public Scene
{
protected:
    // -- Main Scene Properties -- //
    
    // -- //

public:
    /// <summary>
    ///     Constructor.
    /// </summary>
    SceneMain();

    /// <summary>
    ///     Destructor
    /// </summary>
    ~SceneMain();

    /// <summary>
    ///     Called in the constructor to register all Commands.
    /// </summary>
    void registerCommands() override;

    /// <summary>
    ///     Updates scene logic.
    /// </summary>
    /// 
    /// <param name="ctx">Context information that be used by the SceneMain.</param>
    void update(StateContext ctx) override;

    /// <summary>
    ///     Renders scene contents to the Window.
    /// </summary>
    /// 
    /// <param name="ctx">Context information that be used by the SceneMain.</param>
    void render(StateContext ctx) override;
};