#pragma once
#pragma pack(push, 1)

typedef FILE* LPFILE;

/* // for later !
struct LEVEL_DATA
{
    ///LPFILE fp_level;
    ///LPCHAR fp_data;
    int number_meshbase;
    ///int number_meshes;
    ///int number_anims;
    int number_changes;
    int number_ranges;
    int number_commands;
    int number_bones;
    int number_frames;
    int number_objects;
    int number_statics;
};
*/

struct WEAPON_INFO
{
    short lock_angle[4];
    short left_angle[4];
    short right_angle[4];
    short aim_speed;
    short shot_accuracy;
    short gun_height;
    short target_dist;
    char damage;
    char recoil_frame;
    char flash_time;
    char draw_frame;
    short sample_id;
};

struct PISTOL_DEF
{
    short object_number;
    char draw1_anim2;
    char draw1_anim;
    char draw2_anim;
    char recoil_anim;
};


struct BITE_INFO
{
    int x;
    int y;
    int z;
    int mesh;
};

struct AIOBJECT
{
    short object_number;
    short room_number;
    int x;
    int y;
    int z;
    short trigger_flags;
    short flags;
    short y_rot;
    short box_number;
};

struct OBJ_ROTATION
{
    short torso_y;
    short torso_x;
    short head_y;
    short head_x;
};

struct PHD_MATRIX
{
    int m00;
    int m01;
    int m02;
    int m03;
    int m10;
    int m11;
    int m12;
    int m13;
    int m20;
    int m21;
    int m22;
    int m23;
};

struct GAME_VECTOR
{
    int x;
    int y;
    int z;
    short room_number;
    short box_number;
};

struct OBJECT_VECTOR
{
    int x;
    int y;
    int z;
    short data;
    short flags;
};

struct PHD_3DPOS
{
    int x;
    int y;
    int z;
    short x_rot;
    short y_rot;
    short z_rot;
};

struct PHD_VECTOR
{
    int x;
    int y;
    int z;
};

struct FLOOR_INFO
{
    unsigned short index;
    unsigned short fx : 4;
    unsigned short box : 11;
    unsigned short stopper : 1;
    unsigned char pit_room;
    char floor;
    unsigned char sky_room;
    char ceiling;
};

struct LIGHT_INFO
{
    int x;
    int y;
    int z;
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char type;        // LIGHT_FLAGS::
    unsigned char falloff;
    unsigned char intensity;
    float in;
    float out;
    float length;
    float cutoff;
    float x_target;
    float y_target;
    float z_target;
};

struct MESH_INFO
{
    int x;
    int y;
    int z;
    short y_rot;
    short shade;
    short ocb;
    short static_number;
};

struct COLOR_VECTOR
{
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char cd;
};

struct FLOAT_VECTOR
{
    float x;
    float y;
    float z;
};

struct DOOR_INFO
{
    unsigned short room_number;
    short point[3];
    short corner[12];
};

struct DOORS
{
    unsigned short number_door;
    DOOR_INFO doors[256];
};

struct ROOM_INFO
{
    short* data;
    short* door;
    FLOOR_INFO* floor;
    LIGHT_INFO* light;
    MESH_INFO* mesh;
    int x;
    int y;
    int z;
    int minfloor;
    int maxceiling;
    short x_size;
    short y_size;
    unsigned long ambiant;
    short num_light;
    short num_meshes;
    unsigned char reverb_type;
    unsigned char flip_number;
    char mesh_effect;
    char bound_active;
    short left;
    short right;
    short top;
    short bottom;
    short test_left;
    short test_right;
    short test_top;
    short test_bottom;
    short item_number;
    short fx_number;
    short flipped_room;
    unsigned short flags; // ROOM_INFO_Flags
    int vertice_amount;
    int vertice_water_amount;
    int vertice_dry_amount;
    void* pDirect3DVertexBuffer;
    void* pRectangles;
    float room_x;
    float room_y;
    float room_z;
    int vertice_offset;
    void* pTriAndQuad;
    void* pQuadVertice;
    void* pTriVertice;
    int unknown;
    FLOAT_VECTOR* vertice_float;
    int triangle_amount;
    int rectangle_amount;
    void* pLightObject;
};

struct ITEM_INFO
{
    int floor;
    int touch_bits;
    int mesh_bits;
    short object_number;
    short state_current;
    short state_next;
    short state_required;
    short current_anim;
    short current_frame;
    short room_number;
    short next_item;
    short next_free;
    short speed;
    short fallspeed;
    short hit_points;
    unsigned short box_number;
    short timer;
    short flags;                       // For oneshot and code switches (i.e. NOT flags)
    short shade;
    short ocb_bits;
    short carried_item;
    short after_death;
    unsigned short fired_weapon;
    short reserved_1;
    short reserved_2;
    short reserved_3;
    short reserved_4;
    void* data;
    PHD_3DPOS pos;
    unsigned char light_data[5528];
    unsigned int active : 1;
    unsigned int status : 2;
    unsigned int gravity_status : 1;
    unsigned int hit_status : 1;
    unsigned int collidable : 1;
    unsigned int looked_at : 1;
    unsigned int dynamic_light : 1;
    unsigned int poisoned : 1;
    unsigned int ai_bits : 5;
    unsigned int really_active : 1;
    unsigned int in_draw_room : 1;
    int meshswap_meshbits;
    short drawRoom;
    short TOSSPAD;
};

struct COLL_INFO
{
    int mid_floor;              // relative floor and ceiling heights at midpoint
    int mid_ceiling;
    int mid_type;
    int front_floor;            // relative floor and ceiling heights at front
    int front_ceiling;
    int front_type;
    int left_floor;             // relative floor and ceiling heights at front/left side
    int left_ceiling;
    int left_type;
    int right_floor;            // relative floor and ceiling heights at front/right side
    int right_ceiling;
    int right_type;
    int left_floor2;            // relative floor and ceiling heights at back/left side
    int left_ceiling2;
    int left_type2;
    int right_floor2;           // relative floor and ceiling heights at back/right side
    int right_ceiling2;
    int right_type2;
    long radius;                // INPUT Collision Radius...
    long bad_pos, bad_neg;      // INPUT Relative Heights We want to Collide against...
    long bad_ceiling;           // INPUT Relative Bad Ceiling Height...
    PHD_VECTOR shift;           // Shift/Push Values..
    PHD_VECTOR old;             // INPUT Old Positions
    short old_anim_state;       // INPUT old animation
    short old_anim_number;
    short old_frame_number;
    short facing;               // INPUT Angle we are Moving...
    short quadrant;             // Quadrant we are moving ( Not necessarily facing! )
    short coll_type;            // Type of Collision
    short* trigger;             // Trigger info index
    char tilt_x, tilt_z;        // Type of Tilt of Floor
    char hit_by_baddie;         // Flag to indicate Lara has been hit by a Baddie
    char hit_static;            // Flag to indicate Lara has collided with a Static object
    unsigned short slopes_are_walls : 2;   // Treat big slopesUp as walls
    unsigned short slopes_are_pits : 1;    // Treat big slopesDown as pits
    unsigned short lava_is_pit : 1;        // Treat Lava as Bad place to go onto
    unsigned short enable_baddie_push : 1; // Baddies can push Lara
    unsigned short enable_spaz : 1;        // Spaz animations are enabled
    unsigned short hit_ceiling : 1;        // Has Lara hit ceiling? (For up jump only).
};

typedef void(*LPINITIALISE)(short item_number);
typedef void(*LPCONTROL)(short item_number);
typedef void(*LPFLOOR)(ITEM_INFO* item, int x, int y, int z, int* height);
typedef void(*LPCEILING)(ITEM_INFO* item, int x, int y, int z, int* height);
typedef void(*LPDRAW)(ITEM_INFO* item);
typedef void(*LPCOLLISION)(short item_number, ITEM_INFO* laraitem, COLL_INFO* coll);
typedef void(*LPDRAWEXTRA)(ITEM_INFO* item);

struct OBJECT_INFO
{
    short nmeshes;
    short mesh_index;
    int bone_index;
    short* frame_base;
    LPINITIALISE initialise;
    LPCONTROL control;
    LPFLOOR floor;
    LPCEILING ceiling;
    LPDRAW draw_routine;
    LPCOLLISION collision;
    short object_mip;
    short anim_index;
    short hit_points;
    short pivot_length;
    short radius;
    short shadow_size;
    unsigned short bit_offset;
    unsigned short loaded : 1;
    unsigned short intelligent : 1;
    unsigned short non_lot : 1;                // can fly correctly
    unsigned short save_position : 1;
    unsigned short save_hitpoints : 1;
    unsigned short save_flags : 1;
    unsigned short save_anim : 1;
    unsigned short semi_transparent : 1;
    unsigned short water_creature : 1;
    unsigned short using_drawanimating_item : 1;
    unsigned short hit_effect : 2;
    unsigned short undead : 1;
    unsigned short save_mesh : 1;
    unsigned short unknown : 2;
    LPDRAWEXTRA draw_routine_extra;
    unsigned int explodable_meshbits;
    unsigned int pad;
};

struct RANGE_STRUCT
{
    short start_frame;
    short end_frame;
    short link_anim_num;
    short link_frame_num;
};

struct CHANGE_STRUCT
{
    short anim_goal_state;
    short number_ranges;
    short range_index;
};

struct ANIM_STRUCT
{
    short* frame_ptr;
    unsigned char interpolation;
    unsigned char frame_size;
    short current_anim_state;
    int yspeed;
    int yfallspeed;
    int xspeed;
    int xfallspeed;
    short frame_base;
    short frame_end;
    short jump_anim_num;
    short jump_frame_num;
    short number_changes;
    short change_index;
    short number_commands;
    short command_index;
};

struct STATIC_INFO
{
    short mesh_number;
    short flags;
    short x_minp;
    short x_maxp;
    short y_minp;
    short y_maxp;
    short z_minp;
    short z_maxp;
    short x_minc;
    short x_maxc;
    short y_minc;
    short y_maxc;
    short z_minc;
    short z_maxc;
};

struct CAMERA_INFO
{
    GAME_VECTOR src;
    GAME_VECTOR target;
    int type;
    int old_type;
    int shift;
    int flags;
    int fixed_camera;
    int number_frames;
    int bounce;
    int underwater;
    int distance;
    short target_angle;
    short target_elevation;
    short actual_angle;
    short actual_elevation;
    short number;
    short timer;
    short lara_node;
    short speed;
    ITEM_INFO* item;
    ITEM_INFO* last_item;
    OBJECT_VECTOR* fixed;
    int dword_7FE880;
    int dword_7FE884;
    int dword_7FE888;
    int mike_at_lara;
    PHD_VECTOR mike_pos;
    short mike_pos_room;
};

struct LARA_ARM
{
    short *frame_base;
    short frame_curr;
    short anim_curr;
    short lock;
    short y_rot;
    short x_rot;
    short z_rot;
    short flash_gun;
};

struct UnknownStruct
{
    char byte_80E137;
    char byte_80E138;
    char byte_80E139;
};

struct LARA_MESH
{
    short *hips;
    short *thigh_l;
    short *calf_l;
    short *foot_l;
    short *thigh_r;
    short *calf_r;
    short *foot_r;
    short *torso;
    short *uarm_r;
    short *larm_r;
    short *hand_r;
    short *uarm_l;
    short *larm_l;
    short *hand_l;
    short *head;
};

struct FX_INFO
{
    PHD_3DPOS pos;
    short room_number;
    short object_number;
    short next_fx;
    short next_active;
    short speed;
    short fallspeed;
    short current_frame;
    short counter;
    short shade;
    short flag1;
    short flag2;
};

struct DOOR_VBUF
{
    int xv;
    int yv;
    int zv;
};

struct ROPE_STRUCT
{
    PHD_VECTOR seg[24];
    PHD_VECTOR vel[24];
    PHD_VECTOR normal[24];
    PHD_VECTOR mesh[24];
    PHD_VECTOR pos;
    int segLength;
    short active;
    short coiled;
};

struct PENDULUM
{
    PHD_VECTOR pos;
    PHD_VECTOR vel;
    int node;
    ROPE_STRUCT *rope;
};

struct GUNFLASH_STRUCT
{
    PHD_MATRIX matrix;
    short on;
};

struct DYNAMIC
{
    int x;
    int y;
    int z;
    unsigned char on;
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned short falloff;
    unsigned char used;
    unsigned char pad1[1];
    int falloff_scale;
};

struct FIRE_SPARKS
{
    short x;
    short y;
    short z;
    short xvel;
    short yvel;
    short zvel;
    short gravity;
    short rotang;
    short flags;
    unsigned char sSize;
    unsigned char dSize;
    unsigned char Size;
    unsigned char friction;
    unsigned char scalar;
    unsigned char def;
    char rotadd;
    char maxyvel;
    unsigned char on;
    unsigned char sR;
    unsigned char sG;
    unsigned char sB;
    unsigned char dR;
    unsigned char dG;
    unsigned char dB;
    unsigned char R;
    unsigned char G;
    unsigned char B;
    unsigned char colfadespeed;
    unsigned char fadetoblack;
    unsigned char slife;
    unsigned char life;
};

struct GUNSHELL_STRUCT
{
    PHD_3DPOS pos;
    short fallspeed;
    short room_number;
    short speed;
    short counter;
    short direction_xrot;
    short object_number;
};

struct SMOKE_SPARKS
{
    int x;
    int y;
    int z;
    short xvel;
    short yvel;
    short zvel;
    short gravity;
    short rotang;
    short flags;
    unsigned char sSize;
    unsigned char dSize;
    unsigned char size;
    unsigned char friction;
    unsigned char scalar;
    unsigned char def;
    char rotadd;
    char maxyvel;
    unsigned char on;
    unsigned char sShade;
    unsigned char dShade;
    unsigned char shade;
    unsigned char colfadespeed;
    unsigned char fadetoblack;
    char sLife;
    char life;
    unsigned char transtype;
    unsigned char fx_obj;
    unsigned char node_number;
    unsigned char mirror;
};

struct SPARKS
{
    int x;
    int y;
    int z;
    short xvel;
    short yvel;
    short zvel;
    short gravity;
    short rotang;
    short flags;
    unsigned char sSize;
    unsigned char dSize;
    unsigned char size;
    unsigned char friction;
    unsigned char scalar;
    unsigned char def;
    char rotadd;
    char maxyvel;
    unsigned char on;
    unsigned char sR;
    unsigned char sG;
    unsigned char sB;
    unsigned char dR;
    unsigned char dG;
    unsigned char dB;
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char colfadespeed;
    unsigned char fadetoblack;
    unsigned char sLife;
    unsigned char life;
    unsigned char transtype;
    unsigned char extras;
    char dynamic;
    unsigned char fx_obj;
    unsigned char room_number;
    unsigned char node_number;
};

struct BLOOD_STRUCT
{
    int x;
    int y;
    int z;
    short xvel;
    short yvel;
    short zvel;
    short gravity;
    short rotang;
    unsigned char sSize;
    unsigned char dSize;
    unsigned char size;
    unsigned char friction;
    char rotadd;
    unsigned char on;
    unsigned char sShade;
    unsigned char dShade;
    unsigned char shade;
    unsigned char colfadespeed;
    unsigned char fadetoblack;
    char sLife;
    char life;
    char pad;
};

struct SPLASH_STRUCT
{
    int x;
    int y;
    int z;
    short inner_rad;
    short inner_size;
    short inner_radvel;
    short inner_yvel;
    short inner_y;
    short middle_rad;
    short middle_size;
    short middle_radvel;
    short middle_yvel;
    short middle_y;
    short outer_rad;
    short outer_size;
    short outer_radvel;
    char flags;
    unsigned char life;
};

struct BUBBLE_STRUCT
{
    PHD_VECTOR pos;
    short room_number;
    short speed;
    short size;
    short dSize;
    unsigned char shade;
    unsigned char vel;
    unsigned char y_rot;
    char flags;
    short xvel;
    short yvel;
    short zvel;
    short pad;
};

struct DRIP_STRUCT
{
    int x;
    int y;
    int z;
    unsigned char on;
    unsigned char r;
    unsigned char g;
    unsigned char b;
    short yvel;
    unsigned char gravity;
    unsigned char life;
    short room_number;
    unsigned char outside;
    unsigned char pad;
};

struct SHOCKWAVE_STRUCT
{
    int x;
    int y;
    int z;
    short inner_rad;
    short outer_rad;
    short x_rot;
    short flags;
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char life;
    short speed;
    short temp;
};


struct RIPPLE_STRUCT
{
    int x;
    int y;
    int z;
    char flags;
    unsigned char life;
    unsigned char size;
    unsigned char init;
};

struct DEBRIS_STRUCT
{
    void* textinfo;
    int x;
    int y;
    int z;
    short xyz_offset1[3];
    short direction;
    short xyz_offset2[3];
    short speed;
    short xyz_offset3[3];
    short yvel;
    short gravity;
    short room_number;
    unsigned char on;
    unsigned char x_rot;
    unsigned char y_rot;
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char pad[22];
};

struct FIRE_LIST
{
    int x;
    int y;
    int z;
    unsigned char on;
    unsigned char size;
    short room_number;
};

struct BOX_NODE
{
    short exit_box;
    unsigned short search_number;
    short next_expansion;
    short box_number;
};

struct BOX_INFO
{
    unsigned char left;
    unsigned char right;
    unsigned char top;
    unsigned char bottom;
    short height;
    short overlap_index;
};

struct AI_INFO
{
    short zone_number; // NUMBER
    short enemy_zone; // NUMBER
    int distance;
    int ahead;
    int bite;
    short angle;
    short x_angle;
    short enemy_facing;
};

struct LOT_INFO
{
    BOX_NODE *node;
    short head;
    short tail;
    unsigned short search_number;
    unsigned short block_mask;
    short step;
    short drop;
    short zone_count;
    short target_box;
    short required_box;
    short fly;
    unsigned short can_jump : 1;
    unsigned short can_monkey : 1;
    unsigned short is_amphibious : 1;
    unsigned short is_jumping : 1;
    unsigned short is_monkeying : 1;
    PHD_VECTOR target;
    int zone;
};

struct CREATURE_INFO
{
    short joint_rotation[4];
    short maximum_turn;
    short flags;
    unsigned short alerted : 1;
    unsigned short head_left : 1;
    unsigned short head_right : 1;
    unsigned short reached_goal : 1;
    unsigned short hurt_by_lara : 1;
    unsigned short patrol2 : 1;
    unsigned short jump_ahead : 1;
    unsigned short monkey_ahead : 1;
    MOOD_TYPE mood;
    ITEM_INFO *enemy;
    ITEM_INFO ai_target;
    short pad;
    short item_number;
    PHD_VECTOR target;
    LOT_INFO LOT;
};

typedef short(*lpEffect)(int x, int y, int z, short speed, short y_rot, short roomNumber);

struct ENTITY_JUMP
{
    bool can_jump_1click;
    bool can_jump_2click;
};

struct OBJECT_FOUND
{
    short item_number;
    ITEM_INFO* target;
};

struct LARA_INFO
{
    short item_number;
    short gun_status;         // LHS_ enum
    short gun_type;           // LG_ enum
    short gun_request_type;   // LG_ enum
    short gun_last_type;      // LG_ enum
    short calc_fallspeed;
    short water_status;       // LWS_ enum
    short climb_status;
    short pose_count;
    short hit_frame;
    short hit_direction;
    short air;
    short dive_count;
    short death_count;
    short current_active;
    short current_xvel;
    short current_yvel;
    short current_zvel;
    short spaz_effect_count;
    short flare_age;
    short skidoo;
    short weapon_item;
    short back_gun;
    short flare_frame;
    short poisoned;
    short electric;
    unsigned char anxiety;
    unsigned char wet[15];
    WORD flare_control_left : 1; // LOBYTE
    WORD flare_control_right : 1;
    WORD look : 1;
    WORD burn : 1;
    WORD keep_ducked : 1;
    WORD is_moving : 1;
    WORD can_monkey_swing : 1;
    WORD burn_blue : 1;
    WORD burn_smoke : 1;         // HIBYTE
    WORD is_ducked : 1;
    WORD has_fired : 1;
    WORD busy : 1;
    WORD lit_torch : 1;
    WORD is_climbing : 1;
    WORD fired : 1;
    long water_surface_dist;
    PHD_VECTOR last_pos;
    FX_INFO *spaz_effect;
    int mesh_effects;
    LARA_MESH mesh;
    ITEM_INFO *target;
    short target_angles[2];
    short turn_rate;
    short move_angle;
    short head_y_rot;
    short head_x_rot;
    short head_z_rot;
    short torso_y_rot;
    short torso_x_rot;
    short torso_z_rot;
    LARA_ARM l_arm;
    LARA_ARM r_arm;
    WORD holsters;
    CREATURE_INFO *creature;
    long corner_x;
    long corner_z;
    char rope_segment;
    char rope_direction;
    short rope_arc_front;
    short rope_arc_back;
    short rope_last_x;
    short rope_maxX_forward;
    short rope_maxX_backward;
    long rope_dframe;
    long rope_frame;
    WORD rope_framerate;
    WORD rope_y;
    long rope_ptr;
    LPVOID general_ptr;
    int rope_offset;
    DWORD rope_downvel;
    char rope_flag;
    char move_count;
    int rope_count;
    char pistols_type_carried;
    char uzi_type_carried;
    char shotgun_type_carried;
    char crossbow_type_carried;
    char grenadegun_type_carried;
    char sixshooter_type_carried;
    char lasersight;
    char binoculars;
    char crowbar;
    char clockwork_beetle;
    char water_skin_empty1;
    char water_skin_empty2;
    char examine1;
    char examine2;
    char examine3;
    char puzzleitems[12];
    short puzzleitemscombo;
    short keyitems;
    short keyitemscombo;
    short pickupitems;
    short pickupitemscombo;
    short questitems;
    short small_medipack_count;
    short large_medipack_count;
    short flare_count;
    short pistol_ammo_count;
    short uzi_ammo_count;
    short sixshooter_ammo_count;
    short shotgun_ammo1_count;
    short shotgun_ammo2_count;
    short grenade_ammo1_count;
    short grenade_ammo2_count;
    short grenade_ammo3_count;
    short crossbow_ammo1_count;
    short crossbow_ammo2_count;
    short crossbow_ammo3_count;
    char location;
    char blind_timer;
    UnknownStruct unknown;
    short dash_timer;
};

struct BONE
{
    int bone1;
    int bone2;
    int bone3;

    BONE(int* bone, int boneID)
    {
        bone1 = *(bone + 1 + (boneID * 4));
        bone2 = *(bone + 2 + (boneID * 4));
        bone3 = *(bone + 3 + (boneID * 4));
    }

    BONE(int boneX, int boneY, int boneZ)
    {
        bone1 = boneX;
        bone2 = boneY;
        bone3 = boneZ;
    }
};

typedef short**(*lpAddons)(ITEM_INFO* item, OBJECT_INFO* obj, OBJECT_INFO* obj_sec, int meshID);

struct RENDER
{
    int meshID;
    int boneID;
    bool rotHead, rotTorso;
    bool push, pop;
    bool start;
    lpAddons function;

    // start for frame[]
    RENDER(int meshID, bool push, bool pop)
    {
        this->start = true;
        this->boneID = 0;
        this->meshID = meshID;
        this->rotHead = false;
        this->rotTorso = false;
        this->push = push;
        this->pop = pop;
        this->function = NULL;
    }

    RENDER(int boneID, int meshID, bool push, bool pop)
    {
        this->start = false;
        this->boneID = boneID;
        this->meshID = meshID;
        this->rotHead = false;
        this->rotTorso = false;
        this->push = push;
        this->pop = pop;
        this->function = NULL;
    }

    RENDER(int boneID, int meshID, lpAddons function, bool push, bool pop)
    {
        this->start = false;
        this->boneID = boneID;
        this->meshID = meshID;
        this->rotHead = false;
        this->rotTorso = false;
        this->push = push;
        this->pop = pop;
        this->function = function;
    }

    RENDER(int boneID, int meshID, bool rotHead, bool rotTorso, bool push, bool pop)
    {
        this->start = false;
        this->boneID = boneID;
        this->meshID = meshID;
        this->rotHead = rotHead;
        this->rotTorso = rotTorso;
        this->push = push;
        this->pop = pop;
        this->function = NULL;
    }

    RENDER(int boneID, int meshID, lpAddons function, bool rotHead, bool rotTorso, bool push, bool pop)
    {
        this->start = false;
        this->boneID = boneID;
        this->meshID = meshID;
        this->rotHead = rotHead;
        this->rotTorso = rotTorso;
        this->push = push;
        this->pop = pop;
        this->function = function;
    }
};

struct D3DTEXTUREINFO
{
    DDPIXELFORMAT ddpf;
    unsigned long bpp;
    bool bPalette;
    bool bAlpha;
    unsigned char rbpp;
    unsigned char gbpp;
    unsigned char bbpp;
    unsigned char abpp;
    unsigned char rshift;
    unsigned char gshift;
    unsigned char bshift;
    unsigned char ashift;
};

struct DISPLAYMODE
{
    int w;
    int h;
    unsigned long bpp;
    unsigned int mipMapCount;
    bool bPalette;
    DDSURFACEDESC2 ddsd;
    unsigned char rbpp;
    unsigned char gbpp;
    unsigned char bbpp;
    unsigned char rshift;
    unsigned char gshift;
    unsigned char bshift;
};

struct DIRECTSOUNDINFO
{
    char Name[30];
    char About[80];
    LPGUID lpGuid;
    GUID Guid;
};

struct DIRECTDRAWINFO
{
    char Name[30];
    char About[80];
    LPGUID lpGuid;
    GUID Guid;
    DDCAPS DDCaps;
    DDDEVICEIDENTIFIER DDDevIdent;
    int nDisplayMode;
    DISPLAYMODE *DisplayMode;
    int nD3DInfo;
    D3DTEXTUREINFO *D3DInfo;
};

struct DEVICEINFO
{
    int nDDInfo;
    int nDSInfo;
    DIRECTDRAWINFO* DDInfo;
    DIRECTSOUNDINFO* DSInfo;
};

struct DXCONFIG
{
    int nDD;
    int nD3D;
    int nVMode;
    int nTexture;
    int bZbuffer;
    int nSoundCard;
};

struct WINAPP
{
    HINSTANCE hInstance;
    HWND WindowHandle;
    WNDCLASS WindowClass;
    DEVICEINFO DeviceInfo;
    DXCONFIG DXConfig;
    char Unknown;
    LPDIRECTDRAW4 lpDD;
    LPDIRECT3D3 lpD3D;
    LPDIRECT3DDEVICE3 lpD3DDevice;
    LPDIRECT3DDEVICE3 lpD3DDevice_BIS;
    LPDIRECTDRAWSURFACE4 lpFrontBuffer;
    LPDIRECTDRAWSURFACE4 lpBackBuffer;
    LPDIRECTDRAWSURFACE4 lpZBuffer;
    LPDIRECT3DVIEWPORT3 lpViewPort;
    LPDIRECTSOUND lpDS;
    int WinWidth;
    int WinHeight;
    RECT GnWindowRect;
    RECT GnClientRect;
    unsigned int Flags;
    unsigned int DXWndStyle;
    unsigned int DXCoopLevel;
    LPDIRECTINPUT7 lpDI;
    LPDIRECTINPUTDEVICE2A lpDIKeyboard;
    LPDIRECTINPUTDEVICE2A lpDIJoystick;
    BOOL sceneEnabled;
    BOOL bNoFocus;
    BOOL sceneDone;
    HANDLE hObject;
    float fps;
    DWORD dword753A51;
    DWORD dword753A55;
    HACCEL hAccTable;
    BYTE gameInitialized;
    BYTE settingBumpMap;
    DWORD settingTextRes;
    DWORD settingDumpRes;
    BYTE mmxSupport;
    BYTE settingFilter;
    BYTE settingVolumetric;
    BYTE disableSound;
    DWORD settingHardware;
    BYTE playFmv;
    DWORD originalBPP;
    BOOL autoTarget;
};

struct PICKUP_STRUCT
{
    short object_number;
    short timer;
};

struct D3DTLBUMPVERTEX
{
    D3DVALUE sx;
    D3DVALUE sy;
    D3DVALUE sz;
    D3DVALUE rhw;
    D3DCOLOR color;
    D3DCOLOR specular;
    D3DVALUE tu;
    D3DVALUE tv;
    D3DVALUE tu2;
    D3DVALUE tv2;
};

struct DINFO_HEAD
{
    float z;
    short texattr;
    short tile_and_flag;
    short num_verts;
    short unknown;
};

struct DINFO_LINE
{
    DINFO_HEAD head;
    D3DTLBUMPVERTEX v1;
    D3DTLBUMPVERTEX v2;
};

struct DINFO_TRI
{
    DINFO_HEAD head;
    D3DTLBUMPVERTEX v1;
    D3DTLBUMPVERTEX v2;
    D3DTLBUMPVERTEX v3;
};

struct DINFO_QUAD
{
    DINFO_HEAD head;
    D3DTLBUMPVERTEX v1;
    D3DTLBUMPVERTEX v2;
    D3DTLBUMPVERTEX v3;
    D3DTLBUMPVERTEX v1bis;
    D3DTLBUMPVERTEX v3bis;
    D3DTLBUMPVERTEX v4;
};

struct OBJECT_TEXTURE_VERT
{
    float x;
    float y;
};

struct OBJECT_TEXTURE
{
    unsigned short attribute;
    unsigned short tile_and_flag;
    unsigned short new_flags;
    OBJECT_TEXTURE_VERT vertices[4];
};

struct TEXTUREBUCKET
{
    int tpage;
    int cnt;
    D3DTLBUMPVERTEX Vertex[2080];
};

#pragma pack(pop)