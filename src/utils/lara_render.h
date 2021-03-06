#pragma once

class LARA_RENDER
{
private:
    short pprot;
    OBJECT_INFO* obj;
    LARA_MATRIX* mptr;
    int* bone;
    int frac;
    int rate;
    short* frmptr[2];
    short* frame;
    short* rotation1;
    short* rotation2;
    short* rotationw1;
    short* rotationw2;
    short gun_type;
    bool cutsceneMode;

public:
    void initialise(bool isCutscene = false, int* ifCutscene = nullptr, short rotation = 0);
    void check_hitdirection(short* frameCutscene);
    void create_rotation(short* frameCutscene);
    void create_shadow(short* frameCutscene);
    void assign_weapon();

    void start_world();
    void start_list();
    
    void mesh_hips(short* frameCutscene);
    void mesh_thight_l();
    void mesh_calf_l();
    void mesh_foot_l();
    void mesh_thight_r();
    void mesh_calf_r();
    void mesh_foot_r();
    void mesh_torso();
    void mesh_head();
    void mesh_unarmed();
    void mesh_1gun();
    void mesh_2gun();
    void mesh_revolver();

    void end_list();
    void end_world();
};

extern LARA_RENDER lara_render;