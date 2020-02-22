#pragma once

#pragma pack(push, 1)
typedef struct {
    BYTE opCode;    // must be 0xE9;
    DWORD offset;    // jump offset
} JMP;
#pragma pack(pop)

class injector
{
private:
    void inject(DWORD from, LPVOID to);

    /// 3DSYSTEM
    void inject_3d_gen();
    void inject_3d_gen_a();

    /// GAME
    void inject_box();
    void inject_collide();
    void inject_control();
    void inject_debug();
    void inject_delstuff();
    void inject_deltapak();
    void inject_door();
    void inject_draw();
    void inject_effect2();
    void inject_effects();
    void inject_health();
    void inject_items();
    void inject_lara1gun();
    void inject_lara2gun();
    void inject_larafire();
    void inject_laraflare();
    void inject_laramisc();
    void inject_lot();
    void inject_people();
    void inject_pickup();
    void inject_puzzle();
    void inject_rope();
    void inject_sound();
    void inject_sphere();
    void inject_switchs();
    void inject_tomb4fx();
    void inject_traps();

    /// SPECIFIC
    void inject_drawprimitive();
    void inject_file();
    void inject_setup();

public:
    void inject_3dsystem();
    void inject_game();
    void inject_specific();
};

extern injector* inject;