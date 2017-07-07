#include "global.h"
#include "decompress.h"
#include "graphics.h"
#include "item.h"
#include "main.h"
#include "menu.h"
#include "menu_helpers.h"
#include "palette.h"
#include "rom4.h"
#include "sprite.h"
#include "task.h"
#include "text.h"
#include "unknown_task.h"

extern u8 ewram[];

struct UnknownStruct1
{
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
};

extern s8 gUnknown_02038559;
extern u8 gUnknown_0203855A;
extern u8 gUnknown_0203855B;
extern u8 gUnknown_0203855C;
extern u8 gUnknown_03000700;
extern u8 gUnknown_03000701;
extern struct UnknownStruct1 gUnknown_03005D10[];
extern struct ItemSlot *gUnknown_03005D24;

extern void gpu_pal_allocator_reset__manage_upper_four(void);
extern void sub_80F9020(void);
extern void sub_80F9988();

extern const struct CompressedSpriteSheet gUnknown_083C1CC8;
extern const struct CompressedSpriteSheet gUnknown_083C1CD0;
extern const struct CompressedSpritePalette gUnknown_083C1CD8;

void sub_80A34E8(void);
bool8 sub_80A3520(void);
void sub_80A362C(void);
void sub_80A3740(void);
void sub_80A39B8(u16 *, u8);
void sub_80A3AC0(u16 *, u8);
void sub_80A3C34();
void sub_80A3D08(void);
void sub_80A3D40(void);
void sub_80A48E8();
void ItemListMenu_InitDescription();
void ItemListMenu_InitMenu(void);
void sub_80A740C(void);
void sub_80A751C(void);
void sub_80A7630(void);
void sub_80A770C(void);
void sub_80A7828(void);
void sub_80A78B8(void);
void CreateBagSprite(void);
void CreateBagPokeballSprite();

void sub_80A3118(void)
{
    AnimateSprites();
    BuildOamBuffer();
    RunTasks();
    sub_80A740C();
    UpdatePaletteFade();
}

void sub_80A3134(void)
{
    LoadOam();
    ProcessSpriteCopyRequests();
    TransferPlttBuffer();

    {
        const void *src = gBGTilemapBuffers[1];
        void *dst = (void *)(VRAM + 0x2000);

        DmaCopy16(3, src, dst, 0x800);
    }

    {
        const void *src = gBGTilemapBuffers[2];
        void *dst = (void *)(VRAM + 0x6000);

        DmaCopy16(3, src, dst, 0x800);
    }
}

bool8 sub_80A317C(void)
{
    u32 val;
    u16 savedIme;

    switch (gMain.state)
    {
    case 0:
        sub_80F9438();
        sub_80A34E8();
        gMain.state++;
        break;
    case 1:
        remove_some_task();
        gMain.state++;
        break;
    case 2:
        gpu_pal_allocator_reset__manage_upper_four();
        gMain.state++;
        break;
    case 3:
        sub_80F9020();
        ewram[0x1FFFF] = 0;
        gMain.state++;
        break;
    case 4:
        ResetPaletteFade();
        gPaletteFade.bufferTransferDisabled = TRUE;
        gMain.state++;
        break;
    case 5:
        ResetSpriteData();
        gMain.state++;
        break;
    case 6:
        if (sub_80A3520() == FALSE)
            break;
        gMain.state++;
        break;
    case 7:
        SetUpWindowConfig(&gWindowConfig_81E6DFC);
        gMain.state++;
        break;
    case 8:
        MultistepInitMenuWindowBegin(&gWindowConfig_81E6DFC);
        gMain.state++;
        break;
    case 9:
        if (!MultistepInitMenuWindowContinue())
            break;
        gMain.state++;
        break;
    case 10:
        sub_80F944C();
        LoadScrollIndicatorPalette();
        CreateVerticalScrollIndicators(0, 172, 12);
        CreateVerticalScrollIndicators(1, 172, 148);
        CreateVerticalScrollIndicators(2, 28, 88);
        CreateVerticalScrollIndicators(3, 100, 88);
        sub_80F9988(0, 2);
        sub_80F9988(1, 2);
        sub_80F9988(2, 2);
        sub_80F9988(3, 2);
        if (gUnknown_03000701 == 4 || gUnknown_03000701 == 5)
        {
            sub_80F979C(2, 1);
            sub_80F979C(3, 1);
        }
        gMain.state++;
        break;
    case 11:
        gUnknown_0203855A = 16;
        sub_80A39B8(gBGTilemapBuffers[2], gUnknown_02038559 + 1);
        sub_80A3AC0(gBGTilemapBuffers[2], gUnknown_02038559);
        sub_80A3D08();
        // What is this?
        sub_80A3C34(gBagPockets[2].itemSlots, *(u32 *)&gBagPockets[2].capacity);
        sub_80A3C34(gBagPockets[3].itemSlots, *(u32 *)&gBagPockets[3].capacity);
        sub_80A3D40();
        gUnknown_03005D24 = gBagPockets[gUnknown_02038559].itemSlots;
        sub_80A362C();
        gMain.state++;
        break;
    case 12:
        sub_80A48E8(0xFFFF, 0, 7);
        val = gUnknown_03005D10[gUnknown_02038559].unk1 + gUnknown_03005D10[gUnknown_02038559].unk0;
        ItemListMenu_InitDescription((s16)gUnknown_03005D24[val].itemId);
        ItemListMenu_InitMenu();
        gUnknown_0203855B = gUnknown_02038559 + 1;
        gUnknown_0203855C = 0;
        gMain.state++;
        break;
    case 13:
        CreateBagSprite();
        CreateBagPokeballSprite(0);
        sub_80A3740();
        gMain.state++;
        break;
    case 14:
        savedIme = REG_IME;
        REG_IME = 0;
        REG_IE |= INTR_FLAG_VBLANK;
        REG_IME = savedIme;
        REG_DISPSTAT |= 8;
        BeginNormalPaletteFade(0xFFFFFFFF, 0, 16, 0, 0);
        gPaletteFade.bufferTransferDisabled = FALSE;
        gMain.state++;
        break;
    case 15:
        if (sub_8055870() == TRUE)
            break;
        gMain.state++;
        break;
    case 16:
        SetVBlankCallback(sub_80A3134);
        SetMainCallback2(sub_80A3118);
        sub_80A751C();
        sub_80A7630();
        sub_80A770C();
        sub_80A7828();
        sub_80A78B8();
        return TRUE;
    }
    return FALSE;
}

bool8 sub_80A34B4(void)
{
    do
    {
        if (sub_80A317C() == TRUE)
        {
            gUnknown_03000700 = 0;
            ResetTasks();
            return TRUE;
        }
    } while (sub_80F9344() != 1);
    return FALSE;
}

void sub_80A34E8(void)
{
    sub_80F9368();
    REG_BG2CNT = 0x0C06;
    REG_BG1CNT = 0x0405;
    REG_DISPCNT = 0x1740;
    REG_BLDCNT = 0;
}

bool8 sub_80A3520(void)
{
    switch (ewram[0x1FFFF])
    {
    case 0:
        LZDecompressVram(gBagScreen_Gfx, (void *)(VRAM + 0x4000));
        ewram[0x1FFFF]++;
        break;
    case 1:
        CpuCopy16(gUnknown_08E77004, gBGTilemapBuffers[2], 0x800);
        ewram[0x1FFFF]++;
        break;
    case 2:
        if (gSaveBlock2.playerGender == MALE || gUnknown_03000701 == 7)
            LoadCompressedPalette(gBagScreenMale_Pal, 0, 64);
        else
            LoadCompressedPalette(gBagScreenFemale_Pal, 0, 64);
        ewram[0x1FFFF]++;
        break;
    case 3:
        if (gSaveBlock2.playerGender == MALE || gUnknown_03000701 == 7)
            LoadCompressedObjectPic(&gUnknown_083C1CC8);
        else
            LoadCompressedObjectPic(&gUnknown_083C1CD0);
        ewram[0x1FFFF]++;
        break;
    case 4:
        LoadCompressedObjectPalette(&gUnknown_083C1CD8);
        ewram[0x1FFFF] = 0;
        return TRUE;
    }
    return FALSE;
}

void sub_80A362C(void)
{
    u8 i;

    for (i = 0; i < 5; i++)
    {
        u8 r3;

        if (gUnknown_03000701 == 5)
            r3 = gUnknown_03005D10[i].unk2 - 1;
        else
            r3 = gUnknown_03005D10[i].unk2;

        if (gUnknown_03005D10[i].unk1 != 0)
        {
            if (gUnknown_03005D10[i].unk1 + gUnknown_03005D10[i].unk3 > r3)
                gUnknown_03005D10[i].unk1 = r3 - gUnknown_03005D10[i].unk3;
        }
        else
        {
            if (gUnknown_03005D10[i].unk0 > r3)
                gUnknown_03005D10[i].unk0 = r3;
        }
    }
}

void sub_80A3684(void)
{
    u16 i;

    for (i = 0; i < 5; i++)
    {
        gUnknown_03005D10[i].unk0 = 0;
        gUnknown_03005D10[i].unk1 = 0;
        gUnknown_03005D10[i].unk2 = 0;
        gUnknown_03005D10[i].unk3 = 0;
    }
    gUnknown_02038559 = 0;
}

void sub_80A36B8(u16 *a, u8 b, u8 c, u8 d, u8 e)
{
    u16 i;
    u16 j;

    for (i = c; i <= c + e; i++)
    {
        for (j = b; j <= b + d; j++)
        {
            u32 index = j + i * 32;

            a[index] = 0;
        }
    }
}

void ClearBag(void)
{
    u16 i;

    for (i = 0; i < 5; i++)
        ClearItemSlots(gBagPockets[i].itemSlots, gBagPockets[i].capacity);
    sub_80A3684();
}
