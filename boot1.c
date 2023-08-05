#include "boot1.h"



void entry(void)

{
  uint boot1Control;
  undefined8 bootc;
  
  coproc_moveto_Invalidate_Entire_Instruction(0);
  coproc_moveto_Invalidate_Entire_Data_cache(0);
  boot1Control = coproc_movefrom_Control();
  coproc_moveto_Control(boot1Control | 0x1000);
  word_set((undefined4 *)integer_math_tt,(undefined4 *)&__bss_end,0,4);
  bootc = __bootc_from_arm();
  boot1Control = coproc_movefrom_Control();
  coproc_moveto_Control(boot1Control & 0xffffefff);
                    // WARNING: Could not recover jumptable at 0x0d400058. Too many branches
                    // WARNING: Treating indirect jump as call
  (*(code *)bootc)(boot1Control & 0xffffefff);
  return;
}



void word_set(undefined4 *param_1,undefined4 *param_2,undefined4 param_3,int param_4)

{
  for (; param_1 < param_2; param_1 = (undefined4 *)((int)param_1 + param_4)) {
    *param_1 = param_3;
  }
  return;
}



// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void __outputLed(int param_1)

{
  _DAT_0d8000e0 = _DAT_0d8000e0 & 0xff00ffff | (uint)(param_1 << 0x18) >> 8;
  return;
}



void bootFailure(uint cause)

{
  do {
    __outputLed(cause & 0xff);
    busDelay(1000000);
    __outputLed(~cause & 0xff);
    busDelay(1000000);
  } while( true );
}



// WARNING: Globals starting with '_' overlap smaller symbols at the same address

int setupAes(undefined4 *param_1,undefined4 *param_2,uint base)

{
  int pointer;
  int in_lr;
  
  _DAT_0d020000 = 0;
  pointer = 3;
  do {
    _DAT_0d02000c = *param_1;
    param_1 = param_1 + 1;
    pointer = pointer + -1;
  } while (-1 < pointer);
  pointer = 3;
  do {
    _DAT_0d020010 = *param_2;
    param_2 = param_2 + 1;
    pointer = pointer + -1;
  } while (-1 < pointer);
  _DAT_0d020004 = base;
  _DAT_0d020008 = base;
  return in_lr;
}



// WARNING: Removing unreachable block (ram,0x0d400220)
// WARNING: Removing unreachable block (ram,0x0d400224)
// WARNING: Removing unreachable block (ram,0x0d400234)
// WARNING: Globals starting with '_' overlap smaller symbols at the same address

undefined8 runHashAsync(uint *param_1,uint param_2,uint param_3,uint param_4,int param_5)

{
  int *piVar1;
  uint __n;
  undefined4 uVar2;
  undefined4 in_lr;
  undefined local_21;
  
  uVar2 = 0;
  __n = 0;
  if ((param_3 & 0x3f) != 0) {
    uVar2 = 0xfffffff9;
    goto LAB_0d400142;
  }
  if (param_2 == 2) {
    *param_1 = *param_1 + param_4;
LAB_0d400186:
    if (param_4 != __n) {
      ahbMemFlush(1);
      ahbMemRBInvalidate(5);
      _DAT_0d030000 = (param_4 - __n) - 0x40 >> 6 | 0x80000000;
      _DAT_0d030004 = param_3;
    }
    if (param_2 == 3) {
      memset((void *)0xa8000,0,0x80);
      if (__n != 0) {
        __memcpy_from_thumb((void *)0xa8000,(void *)((param_3 + param_4) - __n),__n);
      }
      local_21 = 0x80;
      __memcpy_from_thumb((void *)(__n + 0xa8000),&local_21,1);
      if ((0x40 - __n < 9) && (__n != 0)) {
        piVar1 = (int *)&DAT_000a807c;
      }
      else {
        piVar1 = (int *)&DAT_000a803c;
      }
      *piVar1 = *param_1 << 3;
      do {
      } while ((int)_DAT_0d030000 < 0);
      ahbMemFlush(1);
      ahbMemRBInvalidate(5);
      do {
      } while( true );
    }
  }
  else {
    if (param_2 < 3) {
      if (param_2 == 1) {
        _DAT_0d030000 = 0;
        _DAT_0d030008 = 0x67452301;
        _DAT_0d03000c = 0xefcdab89;
        _DAT_0d030010 = 0x98badcfe;
        _DAT_0d030014 = 0x10325476;
        _DAT_0d030018 = 0xc3d2e1f0;
        *param_1 = param_4;
        if (param_4 == 0) goto LAB_0d400142;
        goto LAB_0d400186;
      }
    }
    else if (param_2 == 3) {
      __n = param_4 & 0x3f;
      *param_1 = *param_1 + param_4;
      goto LAB_0d400186;
    }
    uVar2 = 0xfffffff9;
  }
LAB_0d400142:
  return CONCAT44(in_lr,uVar2);
}



int checkEcc(int buf,int spare,uint pagesize)

{
  uint result;
  int offset;
  uint high;
  uint rc;
  byte local_25;
  
  rc = 0;
  if (pagesize >> 9 != 0) {
    do {
      result = *(uint *)(spare + rc * 4 + 0x40);
      high = *(uint *)(((pagesize >> 5) - ((int)(pagesize >> 5) >> 2)) + spare + rc * 4);
      if ((high != result) &&
         (result = (high >> 0x18 | (high & 0xff0000) >> 8 | (high & 0xff00) << 8 | high << 0x18) ^
                   (result >> 0x18 | (result & 0xff0000) >> 8 | (result & 0xff00) << 8 |
                   result << 0x18), (result - 1 & result) != 0)) {
        high = result >> 0x10;
        if ((result & 0xfff ^ high) != 0xfff) {
          return -1;
        }
        offset = rc * 0x200 + buf;
        result = (high << 0x14) >> 0x17;
        local_25 = (byte)(1 << (high & 7)) ^ *(byte *)(result + offset);
        __memcpy_from_thumb((void *)(result + offset),&local_25,1);
      }
      rc = rc + 1;
    } while (rc < pagesize >> 9);
  }
  return 0;
}



// WARNING: Removing unreachable block (ram,0x0d400394)

undefined4 readPageAsync(undefined4 param_1,undefined4 param_2,undefined4 param_3)

{
                    // WARNING: Do nothing block with infinite loop
  do {
  } while( true );
}



undefined8 getExpSize(int param_1,int param_2)

{
  int iVar1;
  int iVar2;
  undefined4 in_lr;
  
  iVar1 = 0;
  for (iVar2 = param_2 + -1; (-1 < iVar2 && (*(char *)(param_1 + iVar2) == '\0'));
      iVar2 = iVar2 + -1) {
    iVar1 = iVar1 + 1;
  }
  return CONCAT44(in_lr,param_2 - iVar1);
}



int verifySig(void *base,void *p,void *issuer,int pubkey,int exp,uint type,uint size)

{
  int i;
  void *base2;
  uint issuerName;
  uint hashSize;
  undefined8 rc;
  uint ctx;
  byte hash [20];
  
  if (p < base) {
    return -5;
  }
  if ((void *)((int)base + 0x20000) < p) {
    return -5;
  }
  if (0x3fff < size) {
    return -5;
  }
  __memcpy_from_thumb((void *)0xac000,p,size);
  if (type == 2) {
LAB_0d40043a:
    issuerName = 0xac140;
    base2 = (void *)0xac140;
    hashSize = size - 0x140;
  }
  else if (type < 3) {
    if (type != 1) {
      return -8;
    }
    issuerName = 0xac240;
    base2 = (void *)0xac240;
    hashSize = size - 0x240;
  }
  else {
    if (type != 3) {
      if (type != 4) {
        return -8;
      }
      goto LAB_0d40043a;
    }
    issuerName = 0xac140;
    base2 = (void *)0xac140;
    hashSize = size - 0x140;
  }
  if (0x3fff < hashSize) {
    return -5;
  }
  i = memcmp(base2,issuer,0x40);
  if (i != 0) {
    return -8;
  }
  rc = runHashAsync(&ctx,1,issuerName,0,0);
  if ((int)rc < 0) {
    return (int)rc;
  }
  rc = runHashAsync(&ctx,3,issuerName,hashSize,(int)hash);
  if ((int)rc < 0) {
    return (int)rc;
  }
  if (type != 2) {
    if (type < 3) {
      if (type != 1) {
        return -8;
      }
      rc = getExpSize(exp,4);
      rc = CSL_VerifyRsaSig4096(hash,pubkey,0xac004,exp,(int)rc);
      i = (int)rc;
      goto LAB_0d4004a2;
    }
    if ((type != 3) && (type != 4)) {
      return -8;
    }
  }
  rc = getExpSize(exp,4);
  rc = CSL_VerifyRsaSig2048(hash,pubkey,0xac004,exp,(int)rc);
  i = (int)rc;
LAB_0d4004a2:
  if (i != 0) {
    return -8;
  }
  return 0;
}



// WARNING: Removing unreachable block (ram,0x0d400a76)
// WARNING: Removing unreachable block (ram,0x0d400aa2)
// WARNING: Removing unreachable block (ram,0x0d400aa6)
// WARNING: Removing unreachable block (ram,0x0d400e68)
// WARNING: Removing unreachable block (ram,0x0d400e74)
// WARNING: Removing unreachable block (ram,0x0d400e7a)
// WARNING: Removing unreachable block (ram,0x0d400aa8)
// WARNING: Globals starting with '_' overlap smaller symbols at the same address

undefined8 bootc(void)

{
  uint uVar1;
  int iVar2;
  int iVar3;
  int *base;
  uint uVar4;
  uint uVar5;
  uint uVar6;
  uint uVar7;
  int *piVar8;
  uint uVar9;
  uint uVar10;
  undefined *puVar11;
  uint uVar12;
  void *pvVar13;
  void *pvVar14;
  uint uVar15;
  uint uVar16;
  uint uVar17;
  int iVar18;
  uint uVar19;
  int *cause;
  undefined4 in_lr;
  undefined8 uVar20;
  uint local_2b0;
  uint local_2a8;
  uint local_2a4;
  int local_294;
  int *local_28c;
  uint uStack_280;
  int local_27c;
  uint uStack_278;
  uint local_274;
  uint local_270;
  uint uStack_26c;
  uint local_268;
  int local_264 [5];
  undefined4 auStack_250 [4];
  undefined4 auStack_240 [32];
  char acStack_1c0 [64];
  char acStack_180 [64];
  undefined auStack_140 [8];
  int aiStack_138 [14];
  undefined4 auStack_100 [23];
  undefined4 auStack_a4 [4];
  int local_94;
  uint local_90;
  int local_8c;
  int local_88;
  uint local_84;
  undefined auStack_74 [8];
  uint local_6c;
  char acStack_68 [64];
  int local_28;
  
  cause = (int *)0x0;
  __outputLed(0x11);
  EE_GPIOSetup();
  getChipId(&local_268,&uStack_26c);
  if (1 < local_268) {
    uVar20 = EE_RdCmd(0x42,&local_270);
    cause = (int *)uVar20;
    if (cause != (int *)0x0) {
      bootFailure(0xfffffff4);
    }
    uVar1 = isSSConfigValid(local_270 & 0xffff);
    if (((uVar1 & 0xff) != 0) && (local_28 = _DAT_0d8001a0, -1 < _DAT_0d8001a0 << 5)) {
      setSS(local_270 & 0xffff);
      do {
                    // WARNING: Do nothing block with infinite loop
      } while( true );
    }
  }
  getChipId(&local_274,&uStack_278);
  _DAT_0d800060 = 7;
  if (local_274 == 0) {
    _DAT_0d800060 = 0x47;
  }
  _DAT_0d8b0010 = 0;
  init_DDR_Napa(0,'\0');
  uVar1 = 1;
  do {
    if (uVar1 == 2) {
      do {
      } while (_DAT_0d010000 < 0);
      do {
      } while ((int)_DAT_0d020000 < 0);
      do {
      } while (_DAT_0d030000 < 0);
      ahbMemFlush(1);
      ahbMemFlush(3);
      ahbMemFlush(4);
      ahbMemFlush(5);
      ahbMemRBInvalidate(0);
      ahbMemRBInvalidate(3);
      ahbMemRBInvalidate(4);
      ahbMemRBInvalidate(5);
    }
    __memcpy_from_thumb(auStack_140,&DAT_0d4042c0,8);
    local_6c = 0;
    uVar10 = 1;
    do {
      uVar10 = uVar10 + 1;
      readPageAsync(uVar10 * 0x40 + -1,0x28000,&DAT_00028800);
      do {
      } while (_DAT_0d010000 < 0);
      ahbMemFlush(3);
      ahbMemRBInvalidate(0);
      iVar2 = memcmp((void *)0x28000,auStack_140,8);
      if ((iVar2 == 0) &&
         ((iVar2 = memcmp((void *)0x28000,(void *)0x2804c,0x4c), iVar2 == 0 ||
          (iVar2 = memcmp((void *)0x28000,(void *)0x28098,0x4c), iVar2 == 0)))) {
        iVar2 = 0x28000;
        uVar15 = _DAT_00028008;
        if (local_6c < _DAT_00028008) {
LAB_0d4006b6:
          local_6c = uVar15;
          __memcpy_from_thumb(acStack_68,(void *)(iVar2 + 0xc),0x40);
        }
      }
      else {
        iVar2 = memcmp((void *)0x2804c,auStack_140,8);
        if ((iVar2 == 0) &&
           ((iVar2 = memcmp((void *)0x2804c,(void *)0x28098,0x4c), iVar2 == 0 &&
            (iVar2 = 0x2804c, uVar15 = _DAT_00028054, local_6c < _DAT_00028054))))
        goto LAB_0d4006b6;
      }
    } while (uVar10 < 8);
    if (uVar1 == 1) {
      piVar8 = (int *)0x1;
      iVar2 = 1;
    }
    else {
      piVar8 = (int *)0x7;
      iVar2 = -1;
    }
    if (local_6c < 2) {
      if ((int)piVar8 - 1U < 7) {
        do {
          uVar10 = 0;
          do {
            readPageAsync((int)piVar8 * 0x40 + uVar10,0x28000,&DAT_00028800);
            do {
            } while (_DAT_0d010000 < 0);
            ahbMemFlush(3);
            ahbMemRBInvalidate(0);
            if (DAT_00028800 != -1) {
              piVar8 = (int *)((int)piVar8 + iVar2);
              break;
            }
            uVar10 = uVar10 + 1;
          } while (uVar10 < 2);
          if (uVar10 == 2) {
            uVar10 = (int)piVar8 - 1;
            goto LAB_0d400636;
          }
        } while ((int)piVar8 - 1U < 7);
      }
      else {
LAB_0d400bbc:
        uVar10 = (int)piVar8 - 1;
LAB_0d400636:
        if (uVar10 < 7) goto LAB_0d400734;
      }
LAB_0d40063c:
      piVar8 = (int *)0xfffffffc;
LAB_0d400644:
      local_28c = piVar8;
      if (-1 < (int)piVar8) {
LAB_0d4007a0:
        if (uVar1 < 3) goto LAB_0d40065c;
        goto LAB_0d400656;
      }
    }
    else {
      uVar10 = (int)piVar8 - 1;
      if (6 < uVar10) goto LAB_0d400bbc;
      if (acStack_68[(int)piVar8] != '\x01') goto LAB_0d400636;
      do {
        piVar8 = (int *)((int)piVar8 + iVar2);
        if (6 < (int)piVar8 - 1U) goto LAB_0d40063c;
      } while (acStack_68[(int)piVar8] == '\x01');
      if (6 < (int)piVar8 - 1U) goto LAB_0d40063c;
LAB_0d400734:
      if ((int)piVar8 < 0) goto LAB_0d400644;
      readPageAsync((int)piVar8 * 0x40,0x28000,&DAT_00028800);
      do {
      } while (_DAT_0d010000 < 0);
      ahbMemFlush(3);
      ahbMemRBInvalidate(0);
      local_28c = (int *)checkEcc(0x28000,(int)&DAT_00028800,0x800);
      if (local_28c == (int *)0x0) {
        __memcpy_from_thumb(&local_94,(void *)0x28000,0x20);
        if (((((local_94 == 0x20) && (local_8c == 0xa00)) && (local_88 == 0x2a4)) &&
            ((local_84 == 0x208 && (local_90 < 0x20001)))) && ((local_90 & 0x3f) == 0))
        goto LAB_0d400854;
        local_28c = (int *)0xfffffffb;
LAB_0d40079a:
        piVar8 = local_28c;
        if ((int)local_28c < 0) goto LAB_0d40064a;
        goto LAB_0d4007a0;
      }
      if (-1 < (int)local_28c) {
LAB_0d400854:
        uVar15 = local_90 >> 0xb;
        uVar10 = 0;
        do {
          do {
          } while (_DAT_0d010000 < 0);
          ahbMemFlush(3);
          ahbMemRBInvalidate(0);
          if (uVar10 <= uVar15) {
            readPageAsync(uVar10 + (int)piVar8 * 0x40,uVar10 * 0x800 + 0x28000,
                          (uVar10 & 1) * 0x80 + 0x48000);
          }
          if ((uVar10 != 0) &&
             (local_28c = (int *)checkEcc((uVar10 - 1) * 0x800 + 0x28000,
                                          (uVar10 - 1 & 1) * 0x80 + 0x48000,0x800),
             local_28c != (int *)0x0)) goto LAB_0d400af2;
          iVar2 = local_94;
          uVar10 = uVar10 + 1;
        } while (uVar10 <= uVar15 + 1);
        pvVar13 = (void *)(local_94 + 0x28000);
        memset(acStack_180,0,0x40);
        strncpy(acStack_180,"Root",0x10);
        local_28c = (int *)verifySig((void *)0x28000,pvVar13,acStack_180,(int)rootKey,(int)&rootExp,
                                     1,0x400);
        if (local_28c == (int *)0x0) {
          iVar3 = memcmp((char *)(iVar2 + 0x28284),&DAT_0d4043d4,2);
          iVar18 = local_94;
          if (iVar3 != 0) {
LAB_0d400e4e:
            local_28c = (int *)0xfffffff7;
            goto LAB_0d40079a;
          }
          pvVar13 = (void *)(local_94 + 0x28400);
          strncat(acStack_180,"-",2);
          strncat(acStack_180,(char *)(iVar2 + 0x28284),0x10);
          local_28c = (int *)verifySig((void *)0x28000,pvVar13,acStack_180,iVar2 + 0x282c8,
                                       iVar2 + 0x283c8,2,0x300);
          if (local_28c == (int *)0x0) {
            iVar3 = memcmp((char *)(iVar18 + 0x28584),&DAT_0d4043dc,2);
            if (iVar3 != 0) goto LAB_0d400e4e;
            __memcpy_from_thumb(acStack_1c0,acStack_180,0x40);
            pvVar13 = (void *)(local_94 + 0x28000 + local_8c + local_88);
            strncat(acStack_180,"-",2);
            strncat(acStack_180,(char *)(iVar18 + 0x28584),0x10);
            local_28c = (int *)verifySig((void *)0x28000,pvVar13,acStack_180,iVar18 + 0x285c8,
                                         iVar18 + 0x286c8,3,local_84);
            iVar18 = local_94;
            if (local_28c == (int *)0x0) {
              pvVar14 = (void *)(local_94 + 0x28700);
              memset(acStack_180,0,0x40);
              strncpy(acStack_180,acStack_1c0,0x40);
              local_28c = (int *)verifySig((void *)0x28000,pvVar14,acStack_180,iVar2 + 0x282c8,
                                           iVar2 + 0x283c8,2,0x300);
              if (local_28c == (int *)0x0) {
                iVar2 = memcmp((char *)(iVar18 + 0x28884),&DAT_0d4043e0,2);
                if (iVar2 != 0) goto LAB_0d400e4e;
                pvVar14 = (void *)(local_94 + 0x28000 + local_8c);
                strncat(acStack_180,"-",2);
                strncat(acStack_180,(char *)(iVar18 + 0x28884),0x10);
                local_28c = (int *)verifySig((void *)0x28000,pvVar14,acStack_180,iVar18 + 0x288c8,
                                             iVar18 + 0x289c8,4,0x2a4);
                if (local_28c == (int *)0x0) {
                  __memcpy_from_thumb(auStack_250,commonAesKey,0x10);
                  __memcpy_from_thumb((void *)0xb0000,(void *)((int)pvVar14 + 0x1bf),0x10);
                  memset(auStack_240,0,0x10);
                  __memcpy_from_thumb(auStack_240,(void *)((int)pvVar13 + 0x18c),8);
                  setupAes(auStack_250,auStack_240,0xb0000);
                  ahbMemFlush(1);
                  ahbMemRBInvalidate(4);
                  do {
                  } while( true );
                }
              }
            }
          }
        }
LAB_0d400af2:
        if (-1 < (int)local_28c) {
          base = (int *)((local_90 & 0x7ff) + 0x28000);
          setupAes(auStack_a4,auStack_100,(uint)base);
          iVar2 = local_27c;
          local_28c = (int *)0x0;
          uVar10 = local_27c + 0xfU & 0xfffffff0;
          local_2a8 = uVar10 + local_90 + 0x7ff >> 0xb;
          uVar15 = local_2a8 + 0x3f >> 6;
          uVar16 = local_90 >> 0xb;
          local_2a8 = local_2a8 & 0x3f;
          if (local_2a8 == 0) {
            local_2a8 = 0x40;
          }
          uVar4 = local_90 & 0x7ff;
          local_2b0 = uVar10 + uVar4 & 0x7ff;
          if (local_2b0 == 0) {
            local_2b0 = 0x800;
          }
          uVar6 = uVar10 + uVar4 + 0x2887f & 0xffffff80;
          if (0x7ffff < uVar6 + 0x100) {
LAB_0d400e54:
            local_28c = (int *)0xfffffffa;
            goto LAB_0d40079a;
          }
          if (uVar1 == 1) {
            local_294 = 1;
          }
          else {
            local_294 = -1;
          }
          uVar19 = 0;
          if (uVar15 != 0) {
            do {
              if (local_6c < 2) {
                uVar7 = (int)piVar8 - 1;
              }
              else {
                uVar7 = (int)piVar8 - 1;
                if (6 < uVar7) goto LAB_0d400e54;
                puVar11 = (undefined *)((int)piVar8 + (int)auStack_74);
                while (puVar11[0xc] == '\x01') {
                  piVar8 = (int *)((int)piVar8 + local_294);
                  uVar7 = (int)piVar8 - 1;
                  if (6 < uVar7) goto LAB_0d400e54;
                  puVar11 = (undefined *)((int)piVar8 + (int)auStack_74);
                }
              }
              if (6 < uVar7) goto LAB_0d400e54;
              uVar7 = uVar16;
              if (uVar19 != 0) {
                uVar7 = 0;
              }
              local_2a4 = 0x40;
              uVar17 = uVar7;
              if (uVar7 < 0x42) {
                do {
                  if ((uVar17 == 2) && (uVar15 - 1 == uVar19)) {
                    local_2a4 = local_2a8;
                  }
                  iVar18 = (uVar19 * 0x40 + uVar17) - uVar16;
                  do {
                  } while (_DAT_0d010000 < 0);
                  ahbMemFlush(3);
                  ahbMemRBInvalidate(0);
                  if (uVar17 < local_2a4) {
                    readPageAsync((int)piVar8 * 0x40 + uVar17,iVar18 * 0x800 + 0x28000,
                                  (uVar17 & 1) * 0x80 + uVar6);
                  }
                  if (uVar17 < 2) {
                    do {
                    } while (_DAT_0d010000 < 0);
                    ahbMemFlush(3);
                    ahbMemRBInvalidate(0);
                    if (*(char *)((uVar17 & 1) * 0x80 + uVar6) != -1) {
                      uVar12 = local_2a4 + 2;
                      break;
                    }
                    if (uVar17 != 0) goto LAB_0d400d10;
                  }
                  else {
LAB_0d400d10:
                    do {
                    } while ((int)_DAT_0d020000 < 0);
                    ahbMemFlush(4);
                    ahbMemRBInvalidate(0);
                    if ((uVar7 < uVar17) && (uVar17 < local_2a4 + 1)) {
                      local_28c = (int *)checkEcc(iVar18 * 0x800 + 0x27800,
                                                  (uVar17 - 1 & 1) * 0x80 + uVar6,0x800);
                      if ((int)local_28c < 0) goto LAB_0d400d02;
                      if (((uVar19 != 0) || (uVar12 = 0x800 - uVar4, uVar16 + 1 != uVar17)) &&
                         ((uVar15 - 1 != uVar19 || (uVar12 = local_2b0, uVar17 != local_2a4)))) {
                        uVar12 = 0x800;
                      }
                      ahbMemFlush(1);
                      ahbMemRBInvalidate(4);
                      if ((uVar19 == 0) && (uVar7 + 1 == uVar17)) {
                        _DAT_0d020000 = 0x98000000;
                      }
                      else {
                        _DAT_0d020000 = 0x98001000;
                      }
                      _DAT_0d020000 = _DAT_0d020000 | uVar12 - 0x10 >> 4;
                    }
                    do {
                    } while (_DAT_0d030000 < 0);
                    if (uVar7 + 1 < uVar17) {
                      if ((uVar19 == 0) && (uVar16 + 2 == uVar17)) {
                        uVar9 = uVar4 + 0x28000;
                        uVar5 = 1;
                        uVar12 = 0x800 - uVar4;
                      }
                      else if ((uVar15 - 1 == uVar19) && (local_2a4 + 1 == uVar17)) {
                        uVar9 = iVar18 * 0x800 + 0x27000;
                        uVar5 = 3;
                        uVar12 = (local_2b0 + iVar2) - uVar10;
                      }
                      else {
                        uVar9 = iVar18 * 0x800 + 0x27000;
                        uVar5 = 2;
                        uVar12 = 0x800;
                      }
                      uVar20 = runHashAsync(&uStack_280,uVar5,uVar9,uVar12,(int)local_264);
                      local_28c = (int *)uVar20;
                      if ((int)local_28c < 0) goto LAB_0d400d02;
                    }
                  }
                  uVar17 = uVar17 + 1;
                  uVar12 = local_2a4 + 2;
                } while (uVar17 < uVar12);
              }
              else {
                uVar12 = 0x42;
              }
              if (uVar12 == uVar17) {
                uVar19 = uVar19 + 1;
              }
              piVar8 = (int *)((int)piVar8 + local_294);
            } while (uVar19 < uVar15);
          }
          uVar10 = 0;
          iVar2 = 0;
          do {
            if (*(int *)((int)aiStack_138 + iVar2) != *(int *)((int)local_264 + iVar2)) {
              local_28c = (int *)0xfffffff9;
              goto LAB_0d40079a;
            }
            uVar10 = uVar10 + 1;
            iVar2 = iVar2 + 4;
          } while (uVar10 < 5);
LAB_0d400d02:
          piVar8 = base;
          if (-1 < (int)local_28c) goto LAB_0d400644;
        }
      }
    }
LAB_0d40064a:
    uVar1 = uVar1 + 1;
    piVar8 = local_28c;
    cause = local_28c;
    if (2 < uVar1) {
LAB_0d400656:
      bootFailure((uint)cause);
LAB_0d40065c:
      piVar8[3] = 1;
      __outputLed(0x22);
      return CONCAT44(in_lr,*piVar8 + (int)piVar8);
    }
  } while( true );
}



// WARNING: Globals starting with '_' overlap smaller symbols at the same address

undefined4 EE_GPIOSetup(void)

{
  undefined4 in_lr;
  
  initGPIO();
  _DAT_0d8000f4 = _DAT_0d8000f4 & 0xffffc3ff;
  _DAT_0d8000e0 = _DAT_0d8000e0 & 0xffffc3ff;
  _DAT_0d8000e4 = _DAT_0d8000e4 | 0x1c00;
  _DAT_0d8000dc = _DAT_0d8000dc | 0x3c00;
  return in_lr;
}



// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void eeClkHigh(void)

{
  _DAT_0d8000e0 = _DAT_0d8000e0 | 0x800;
  return;
}



// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void eeClkLow(void)

{
  _DAT_0d8000e0 = _DAT_0d8000e0 & 0xfffff7ff;
  return;
}



// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void eeCSHigh(void)

{
  _DAT_0d8000e0 = _DAT_0d8000e0 | 0x400;
  return;
}



// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void eeDataOut(uint param_1)

{
  _DAT_0d8000e0 = _DAT_0d8000e0 & 0xffffefff | (param_1 & 1) << 0xc;
  return;
}



// WARNING: Globals starting with '_' overlap smaller symbols at the same address

undefined4 eeBitTimeIdle(int param_1)

{
  undefined4 in_lr;
  
  if (param_1 != 0) {
    do {
      eeClkLow();
      _DAT_0d8000e0 = _DAT_0d8000e0 & 0xfffffbff;
      eeDataOut(0);
      busDelay(0x7d);
      param_1 = param_1 + -1;
      eeClkHigh();
      busDelay(0x7d);
    } while (param_1 != 0);
  }
  eeClkLow();
  return in_lr;
}



undefined8 eeWrNbits(int param_1,uint param_2)

{
  uint uVar1;
  undefined4 uVar2;
  undefined4 in_lr;
  bool bVar3;
  
  uVar1 = param_1 - 1;
  uVar2 = 0;
  if (uVar1 < 0x20) {
    if (uVar1 != 0xffffffff) {
      do {
        eeClkLow();
        eeCSHigh();
        eeDataOut(param_2 >> (uVar1 & 0xff) & 1);
        busDelay(0x7d);
        eeClkHigh();
        busDelay(0x7d);
        bVar3 = uVar1 != 0;
        uVar1 = uVar1 - 1;
      } while (bVar3);
    }
  }
  else {
    uVar2 = 0xfffffffc;
  }
  return CONCAT44(in_lr,uVar2);
}



// WARNING: Globals starting with '_' overlap smaller symbols at the same address

undefined8 eeRdNbits(int param_1,uint *param_2)

{
  uint uVar1;
  uint uVar2;
  undefined4 uVar3;
  undefined4 in_lr;
  bool bVar4;
  
  uVar1 = param_1 - 1;
  uVar3 = 0;
  if (uVar1 < 0x20) {
    uVar2 = 0;
    if (uVar1 != 0xffffffff) {
      do {
        eeClkLow();
        eeCSHigh();
        busDelay(0x7d);
        eeClkHigh();
        busDelay(0x7d);
        uVar2 = uVar2 | ((uint)(_DAT_0d8000e8 << 0x12) >> 0x1f) << (uVar1 & 0xff);
        bVar4 = uVar1 != 0;
        uVar1 = uVar1 - 1;
      } while (bVar4);
    }
    *param_2 = uVar2;
  }
  else {
    uVar3 = 0xfffffffc;
  }
  return CONCAT44(in_lr,uVar3);
}



undefined8 eeChkStatus(void)

{
  undefined4 uVar1;
  uint uVar2;
  undefined4 in_lr;
  undefined8 uVar3;
  uint local_18;
  
  uVar2 = 0;
  local_18 = 0;
  do {
    uVar3 = eeRdNbits(10,&local_18);
    uVar1 = (undefined4)uVar3;
    uVar2 = uVar2 + 1;
    if ((~local_18 & 1) == 0) break;
  } while (uVar2 < 100);
  eeBitTimeIdle(2);
  if (99 < uVar2) {
    uVar1 = 0xfffffff7;
  }
  return CONCAT44(in_lr,uVar1);
}



undefined8 EE_RdCmd(uint param_1,uint *param_2)

{
  int iVar1;
  undefined4 in_lr;
  undefined8 uVar2;
  
  uVar2 = eeWrNbits(0xb,param_1 & 0xff | 0x600);
  iVar1 = (int)uVar2;
  if (iVar1 == 0) {
    uVar2 = eeRdNbits(0x10,param_2);
    iVar1 = (int)uVar2;
    if (iVar1 == 0) {
      eeBitTimeIdle(2);
    }
  }
  return CONCAT44(in_lr,iVar1);
}



undefined8 EE_WrCmd(uint param_1,uint param_2)

{
  int iVar1;
  undefined4 in_lr;
  undefined8 uVar2;
  
  uVar2 = eeWrNbits(0x1b,(param_1 & 0xff | 0x500) * 0x10000 | param_2 & 0xffff);
  iVar1 = (int)uVar2;
  if (iVar1 == 0) {
    eeBitTimeIdle(2);
    uVar2 = eeChkStatus();
    iVar1 = (int)uVar2;
  }
  return CONCAT44(in_lr,iVar1);
}



undefined8 EE_EraseCmd(uint param_1)

{
  int iVar1;
  undefined4 in_lr;
  undefined8 uVar2;
  
  uVar2 = eeWrNbits(0xb,param_1 & 0xff | 0x700);
  iVar1 = (int)uVar2;
  if (iVar1 == 0) {
    eeBitTimeIdle(2);
    uVar2 = eeChkStatus();
    iVar1 = (int)uVar2;
  }
  return CONCAT44(in_lr,iVar1);
}



undefined8 EE_WralCmd(uint param_1)

{
  int iVar1;
  undefined4 in_lr;
  undefined8 uVar2;
  
  uVar2 = eeWrNbits(0x1b,param_1 & 0xffff | 0x4400000);
  iVar1 = (int)uVar2;
  if (iVar1 == 0) {
    eeBitTimeIdle(2);
    uVar2 = eeChkStatus();
    iVar1 = (int)uVar2;
  }
  return CONCAT44(in_lr,iVar1);
}



undefined8 EE_EralCmd(void)

{
  int iVar1;
  undefined4 in_lr;
  undefined8 uVar2;
  
  uVar2 = eeWrNbits(0xb,0x480);
  iVar1 = (int)uVar2;
  if (iVar1 == 0) {
    eeBitTimeIdle(2);
    uVar2 = eeChkStatus();
    iVar1 = (int)uVar2;
  }
  return CONCAT44(in_lr,iVar1);
}



undefined8 EE_WenCmd(void)

{
  undefined4 in_lr;
  undefined8 uVar1;
  
  uVar1 = eeWrNbits(0xb,0x4c0);
  if ((int)uVar1 == 0) {
    eeBitTimeIdle(2);
  }
  return CONCAT44(in_lr,(int)uVar1);
}



undefined8 EE_WdsCmd(void)

{
  undefined4 in_lr;
  undefined8 uVar1;
  
  uVar1 = eeWrNbits(0xb,0x400);
  if ((int)uVar1 == 0) {
    eeBitTimeIdle(2);
  }
  return CONCAT44(in_lr,(int)uVar1);
}



undefined8 eeCommitMainData(ushort *param_1,uint param_2)

{
  ushort uVar1;
  int iVar2;
  uint uVar3;
  undefined4 in_lr;
  undefined8 uVar4;
  
  if (param_2 < 2) {
    uVar4 = EE_WenCmd();
    iVar2 = (int)uVar4;
    if (iVar2 == 0) {
      uVar3 = 0;
      do {
        uVar1 = *param_1;
        param_1 = param_1 + 1;
        uVar4 = EE_WrCmd((param_2 * 10 + 0x48 >> 1) + uVar3,(uint)uVar1);
        iVar2 = (int)uVar4;
        if (iVar2 != 0) break;
        uVar3 = uVar3 + 1;
      } while (uVar3 < 5);
      EE_WdsCmd();
    }
  }
  else {
    iVar2 = -4;
  }
  return CONCAT44(in_lr,iVar2);
}



ulonglong eeComputeMainDataSum(ushort *param_1)

{
  uint uVar1;
  int iVar2;
  uint uVar3;
  undefined4 in_lr;
  
  uVar1 = 0;
  iVar2 = 3;
  do {
    uVar3 = uVar1 + *param_1;
    param_1 = param_1 + 1;
    uVar1 = uVar3 & 0xffff;
    iVar2 = iVar2 + -1;
  } while (-1 < iVar2);
  return CONCAT44(in_lr,uVar3) & 0xffffffff0000ffff;
}



ulonglong eeComputeSfsDataSum(ushort *param_1)

{
  uint uVar1;
  int iVar2;
  uint uVar3;
  undefined4 in_lr;
  
  uVar1 = 0;
  iVar2 = 1;
  do {
    uVar3 = uVar1 + *param_1;
    param_1 = param_1 + 1;
    uVar1 = uVar3 & 0xffff;
    iVar2 = iVar2 + -1;
  } while (-1 < iVar2);
  return CONCAT44(in_lr,uVar3) & 0xffffffff0000ffff;
}



undefined8 eeRetrieveMainData(void *param_1,uint *param_2)

{
  int iVar1;
  uint uVar2;
  undefined2 *puVar3;
  ushort *puVar4;
  uint uVar5;
  uint uVar6;
  int iVar7;
  uint uVar8;
  undefined4 in_lr;
  undefined8 uVar9;
  ulonglong uVar10;
  ushort *local_4c;
  uint local_48;
  uint local_3c;
  ushort local_38 [10];
  undefined4 uStack_4;
  
  uVar8 = 0xffffffff;
  local_48 = 0;
  local_4c = (ushort *)0x0;
  uVar6 = 0;
  iVar1 = 0;
  uStack_4 = in_lr;
  do {
    puVar3 = (undefined2 *)((int)local_38 + iVar1);
    if (1 < uVar6) {
      iVar7 = -4;
      goto LAB_0d4012d6;
    }
    uVar5 = 0;
    do {
      uVar9 = EE_RdCmd((iVar1 + 0x48U >> 1) + uVar5,&local_3c);
      iVar7 = (int)uVar9;
      *puVar3 = (short)local_3c;
      puVar3 = puVar3 + 1;
      if (iVar7 != 0) {
        if (iVar7 != 0) goto LAB_0d4012d6;
        goto LAB_0d401286;
      }
      uVar5 = uVar5 + 1;
    } while (uVar5 < 5);
    iVar7 = 0;
LAB_0d401286:
    uVar6 = uVar6 + 1;
    iVar1 = iVar1 + 10;
  } while (uVar6 < 2);
  uVar5 = 0;
  puVar4 = local_38;
  iVar1 = 0;
  uVar6 = uVar8;
  do {
    uVar10 = eeComputeMainDataSum((ushort *)((int)local_38 + iVar1));
    if ((uint)puVar4[4] == (uint)uVar10) {
      uVar2 = (uint)*(byte *)((int)puVar4 + 7) |
              (uint)*(byte *)(puVar4 + 3) << 8 |
              (uint)*(byte *)((int)puVar4 + 5) << 0x10 | (uint)*(byte *)(puVar4 + 2) << 0x18;
      if (local_48 <= uVar2) {
        local_4c = (ushort *)((int)local_38 + iVar1);
        local_48 = uVar2;
      }
      if (uVar2 < uVar6) {
        uVar8 = uVar5;
        uVar6 = uVar2;
      }
    }
    else {
      uVar8 = uVar5;
      uVar6 = 0;
    }
    uVar5 = uVar5 + 1;
    iVar1 = iVar1 + 10;
    puVar4 = puVar4 + 5;
  } while (uVar5 < 2);
  if ((local_4c == (ushort *)0x0) || ((int)uVar8 < 0)) {
    iVar7 = -5;
  }
  else {
    __memcpy_from_thumb(param_1,local_4c,10);
    *param_2 = uVar8;
  }
LAB_0d4012d6:
  return CONCAT44(uStack_4,iVar7);
}



undefined8 eeRetrieveSfsData(void *param_1,uint *param_2)

{
  int iVar1;
  uint uVar2;
  undefined2 *puVar3;
  ushort *puVar4;
  uint uVar5;
  uint uVar6;
  int iVar7;
  uint uVar8;
  undefined4 in_lr;
  undefined8 uVar9;
  ulonglong uVar10;
  ushort *local_4c;
  uint local_48;
  uint local_3c;
  ushort local_38 [10];
  undefined4 uStack_4;
  
  uVar8 = 0xffffffff;
  local_48 = 0;
  local_4c = (ushort *)0x0;
  uVar6 = 0;
  iVar1 = 0;
  uStack_4 = in_lr;
  do {
    puVar3 = (undefined2 *)((int)local_38 + iVar1);
    if (2 < uVar6) {
      iVar7 = -4;
      goto LAB_0d4013d6;
    }
    uVar5 = 0;
    do {
      uVar9 = EE_RdCmd((iVar1 + 0x5cU >> 1) + uVar5,&local_3c);
      iVar7 = (int)uVar9;
      *puVar3 = (short)local_3c;
      puVar3 = puVar3 + 1;
      if (iVar7 != 0) {
        if (iVar7 != 0) goto LAB_0d4013d6;
        goto LAB_0d401386;
      }
      uVar5 = uVar5 + 1;
    } while (uVar5 < 3);
    iVar7 = 0;
LAB_0d401386:
    uVar6 = uVar6 + 1;
    iVar1 = iVar1 + 6;
  } while (uVar6 < 3);
  uVar5 = 0;
  puVar4 = local_38;
  iVar1 = 0;
  uVar6 = uVar8;
  do {
    uVar10 = eeComputeSfsDataSum((ushort *)((int)local_38 + iVar1));
    if ((uint)puVar4[2] == (uint)uVar10) {
      uVar2 = (uint)*(byte *)((int)puVar4 + 3) |
              (uint)*(byte *)(puVar4 + 1) << 8 |
              (uint)*(byte *)((int)puVar4 + 1) << 0x10 | (uint)*(byte *)puVar4 << 0x18;
      if (local_48 <= uVar2) {
        local_4c = (ushort *)((int)local_38 + iVar1);
        local_48 = uVar2;
      }
      if (uVar2 < uVar6) {
        uVar8 = uVar5;
        uVar6 = uVar2;
      }
    }
    else {
      uVar8 = uVar5;
      uVar6 = 0;
    }
    uVar5 = uVar5 + 1;
    iVar1 = iVar1 + 6;
    puVar4 = puVar4 + 3;
  } while (uVar5 < 3);
  if ((local_4c == (ushort *)0x0) || ((int)uVar8 < 0)) {
    iVar7 = -5;
  }
  else {
    __memcpy_from_thumb(param_1,local_4c,6);
    *param_2 = uVar8;
  }
LAB_0d4013d6:
  return CONCAT44(uStack_4,iVar7);
}



undefined8 EE_GetOsVersion(void)

{
  uint uVar1;
  undefined4 in_lr;
  undefined8 uVar2;
  uint uStack_18;
  byte local_14 [12];
  
  uVar2 = eeRetrieveMainData(local_14,&uStack_18);
  uVar1 = (uint)uVar2;
  if (uVar1 == 0) {
    uVar1 = (uint)local_14[0];
  }
  return CONCAT44(in_lr,uVar1);
}



undefined8 EE_GetCaCrlVersion(void)

{
  uint uVar1;
  undefined4 in_lr;
  undefined8 uVar2;
  uint uStack_18;
  undefined uStack_14;
  byte local_13;
  
  uVar2 = eeRetrieveMainData(&uStack_14,&uStack_18);
  uVar1 = (uint)uVar2;
  if (uVar1 == 0) {
    uVar1 = (uint)local_13;
  }
  return CONCAT44(in_lr,uVar1);
}



undefined8 EE_GetSignerCrlVersion(void)

{
  uint uVar1;
  undefined4 in_lr;
  undefined8 uVar2;
  uint uStack_18;
  undefined auStack_14 [2];
  byte local_12;
  
  uVar2 = eeRetrieveMainData(auStack_14,&uStack_18);
  uVar1 = (uint)uVar2;
  if (uVar1 == 0) {
    uVar1 = (uint)local_12;
  }
  return CONCAT44(in_lr,uVar1);
}



undefined8 EE_GetSfsVersion(void)

{
  int iVar1;
  undefined4 in_lr;
  undefined8 uVar2;
  uint uStack_10;
  int local_c [2];
  undefined4 uStack_4;
  
  uStack_4 = in_lr;
  uVar2 = eeRetrieveSfsData(local_c,&uStack_10);
  iVar1 = (int)uVar2;
  if ((int)uVar2 == 0) {
    iVar1 = local_c[0];
  }
  return CONCAT44(uStack_4,iVar1);
}



undefined8 EE_IncOsVersion(void)

{
  int iVar1;
  undefined4 in_lr;
  undefined8 uVar2;
  ulonglong uVar3;
  uint local_18;
  char local_14;
  int local_10;
  undefined2 local_c;
  
  uVar2 = eeRetrieveMainData(&local_14,&local_18);
  iVar1 = (int)uVar2;
  if (iVar1 == 0) {
    local_14 = local_14 + '\x01';
    local_10 = local_10 + 1;
    uVar3 = eeComputeMainDataSum((ushort *)&local_14);
    local_c = (undefined2)uVar3;
    uVar2 = eeCommitMainData((ushort *)&local_14,local_18);
    iVar1 = (int)uVar2;
  }
  return CONCAT44(in_lr,iVar1);
}



undefined8 EE_IncCaCrlVersion(void)

{
  int iVar1;
  undefined4 in_lr;
  undefined8 uVar2;
  ulonglong uVar3;
  uint local_18;
  undefined2 uStack_14;
  int local_10;
  undefined2 local_c;
  
  uVar2 = eeRetrieveMainData(&uStack_14,&local_18);
  iVar1 = (int)uVar2;
  if (iVar1 == 0) {
    uStack_14._1_1_ = (char)uStack_14 + '\x01';
    local_10 = local_10 + 1;
    uVar3 = eeComputeMainDataSum(&uStack_14);
    local_c = (undefined2)uVar3;
    uVar2 = eeCommitMainData(&uStack_14,local_18);
    iVar1 = (int)uVar2;
  }
  return CONCAT44(in_lr,iVar1);
}



undefined8 EE_IncSignerCrlVersion(void)

{
  int iVar1;
  undefined4 in_lr;
  undefined8 uVar2;
  ulonglong uVar3;
  uint local_18;
  ushort uStack_14;
  char local_12;
  int local_10;
  undefined2 local_c;
  
  uVar2 = eeRetrieveMainData(&uStack_14,&local_18);
  iVar1 = (int)uVar2;
  if (iVar1 == 0) {
    local_12 = local_12 + '\x01';
    local_10 = local_10 + 1;
    uVar3 = eeComputeMainDataSum(&uStack_14);
    local_c = (undefined2)uVar3;
    uVar2 = eeCommitMainData(&uStack_14,local_18);
    iVar1 = (int)uVar2;
  }
  return CONCAT44(in_lr,iVar1);
}



undefined8 EE_IncSfsVersion(void)

{
  ushort uVar1;
  int iVar2;
  int *piVar3;
  uint uVar4;
  undefined4 in_lr;
  undefined8 uVar5;
  ulonglong uVar6;
  uint local_20;
  int local_1c;
  undefined2 local_18;
  
  piVar3 = &local_1c;
  uVar5 = eeRetrieveSfsData(piVar3,&local_20);
  iVar2 = (int)uVar5;
  if (iVar2 == 0) {
    local_1c = local_1c + 1;
    uVar6 = eeComputeSfsDataSum((ushort *)piVar3);
    local_18 = (undefined2)uVar6;
    if (local_20 < 3) {
      uVar5 = EE_WenCmd();
      iVar2 = (int)uVar5;
      if (iVar2 == 0) {
        uVar4 = 0;
        do {
          uVar1 = *(ushort *)piVar3;
          piVar3 = (int *)((int)piVar3 + 2);
          uVar5 = EE_WrCmd((local_20 * 6 + 0x5c >> 1) + uVar4,(uint)uVar1);
          iVar2 = (int)uVar5;
          if (iVar2 != 0) break;
          uVar4 = uVar4 + 1;
        } while (uVar4 < 3);
        EE_WdsCmd();
      }
    }
    else {
      iVar2 = -4;
    }
  }
  return CONCAT44(in_lr,iVar2);
}



undefined8 EE_GetRngState(void)

{
  uint uVar1;
  undefined4 in_lr;
  undefined8 uVar2;
  uint local_c;
  uint local_8;
  undefined4 uStack_4;
  
  uStack_4 = in_lr;
  uVar2 = EE_RdCmd(0x7c,&local_8);
  uVar1 = (uint)uVar2;
  if (uVar1 == 0) {
    uVar2 = EE_RdCmd(0x7d,&local_c);
    uVar1 = (uint)uVar2;
    if (uVar1 == 0) {
      uVar1 = local_8 | local_c << 0x10;
    }
  }
  return CONCAT44(uStack_4,uVar1);
}



undefined8 EE_IncRngState(void)

{
  int iVar1;
  undefined4 in_lr;
  undefined8 uVar2;
  uint local_10;
  uint local_c;
  
  uVar2 = EE_RdCmd(0x7c,&local_c);
  iVar1 = (int)uVar2;
  if (iVar1 == 0) {
    uVar2 = EE_RdCmd(0x7d,&local_10);
    iVar1 = (int)uVar2;
    if (iVar1 == 0) {
      local_c = (local_c | local_10 << 0x10) + 1;
      local_10 = local_c >> 0x10;
      local_c = local_c & 0xffff;
      uVar2 = EE_WenCmd();
      iVar1 = (int)uVar2;
      if (iVar1 == 0) {
        uVar2 = EE_WrCmd(0x7c,local_c);
        iVar1 = (int)uVar2;
        if (iVar1 == 0) {
          uVar2 = EE_WrCmd(0x7d,local_10);
          iVar1 = (int)uVar2;
        }
        EE_WdsCmd();
      }
    }
  }
  return CONCAT44(in_lr,iVar1);
}



// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void getChipId(uint *param_1,uint *param_2)

{
  uint uVar1;
  
  uVar1 = _DAT_0d800214;
  *param_1 = (_DAT_0d800214 << 0x18) >> 0x1c;
  *param_2 = uVar1 & 0xf;
  return;
}



// WARNING: Globals starting with '_' overlap smaller symbols at the same address

undefined8 _busRdAHMReg(uint param_1)

{
  uint uVar1;
  undefined4 in_lr;
  
  uVar1 = 0;
  if ((_DAT_0d800214 & 0xf0) == 0) {
    uVar1 = _DAT_0d800130;
    _DAT_0d800130 = _DAT_0d800130 | 0x400;
  }
  if ((_DAT_0d800214 & 0xf0) == 0) {
    _DAT_0d800130 = uVar1;
  }
  return CONCAT44(in_lr,*(undefined4 *)(param_1 | 0xd8b0000));
}



// WARNING: Globals starting with '_' overlap smaller symbols at the same address

undefined8 rdbf(int param_1)

{
  undefined *puVar1;
  uint uVar2;
  undefined4 in_lr;
  undefined8 uVar3;
  
  if (param_1 == 0xc) {
    uVar2 = 0;
  }
  else if (param_1 == 0xb) {
    uVar2 = 0x1000;
  }
  else if (param_1 == 0) {
    uVar2 = 0x8000;
  }
  else if (param_1 == 1) {
    uVar2 = 0x4000;
  }
  else if (param_1 == 2) {
    uVar2 = 1;
  }
  else if (param_1 == 3) {
    uVar2 = 2;
  }
  else if (param_1 == 4) {
    uVar2 = 4;
  }
  else if (param_1 == 5) {
    uVar2 = 8;
  }
  else if (param_1 == 6) {
    uVar2 = 0x10;
  }
  else if (param_1 == 7) {
    uVar2 = 0x20;
  }
  else if (param_1 == 8) {
    uVar2 = 0x40;
  }
  else if (param_1 == 9) {
    uVar2 = 0x80;
  }
  else {
    uVar2 = 0x100;
  }
  uVar3 = _busRdAHMReg(8);
  puVar1 = (undefined *)((ulonglong)uVar3 >> 0x20);
  if (((uint)uVar3 & uVar2) == 0) {
    if (param_1 - 2U < 9) {
      do {
      } while ((_DAT_0d80018c & 0xf) == 9);
      _DAT_0d800188 = _DAT_0d800188 & 0xfffeffff | 0x2000000;
      _DAT_0d800124 = _DAT_0d800124 & 0xfffff83f | 0x280;
      _DAT_0d800134 = _DAT_0d800134 | 0x400;
      do {
      } while ((_DAT_0d80018c & 0xf) != 9);
      _DAT_0d800100 = _DAT_0d800100 | 0x400;
      _DAT_0d800104 = _DAT_0d800104 | 0x400;
      _DAT_0d800108 = _DAT_0d800108 | 0x400;
      _DAT_0d80010c = _DAT_0d80010c | 0x400;
      _DAT_0d800110 = _DAT_0d800110 | 0x400;
      _DAT_0d800114 = _DAT_0d800114 | 0x400;
      _DAT_0d800118 = _DAT_0d800118 | 0x400;
      _DAT_0d80011c = _DAT_0d80011c | 0x400;
      _DAT_0d800120 = _DAT_0d800120 | 0x400;
      uVar3 = _busRdAHMReg(8);
      _DAT_0d8b0008 = (uint)uVar3 & ~uVar2;
      uVar3 = _busRdAHMReg(8);
      _DAT_0d8b0008 = uVar2 | (uint)uVar3;
      _DAT_0d800134 = _DAT_0d800134 & 0xfffffbff;
      _DAT_0d800100 = _DAT_0d800100 & 0xfffffbff;
      _DAT_0d800104 = _DAT_0d800104 & 0xfffffbff;
      _DAT_0d800108 = _DAT_0d800108 & 0xfffffbff;
      _DAT_0d80010c = _DAT_0d80010c & 0xfffffbff;
      _DAT_0d800110 = _DAT_0d800110 & 0xfffffbff;
      _DAT_0d800114 = _DAT_0d800114 & 0xfffffbff;
      _DAT_0d800118 = _DAT_0d800118 & 0xfffffbff;
      _DAT_0d80011c = _DAT_0d80011c & 0xfffffbff;
      _DAT_0d800120 = _DAT_0d800120 & 0xfffffbff;
      _DAT_0d800188 = _DAT_0d800188 & 0xfdffffff;
      puVar1 = &DAT_0d800124;
      _DAT_0d800124 = _DAT_0d800124 & 0xfffff83f | 0xc0;
    }
    else if (((param_1 == 0xb) || (param_1 == 0)) || (param_1 == 1)) {
      _DAT_0d8b0008 = (uint)uVar3 | uVar2;
    }
  }
  return CONCAT44(puVar1,in_lr);
}



uint busRdMEMReg(uint a)

{
  ushort value;
  
  value = *(ushort *)(a | 0xd8b4000);
  rdbf(0);
  return (uint)value;
}



// WARNING: Globals starting with '_' overlap smaller symbols at the same address

int ahbMemFlush(int groupname)

{
  uint uVar1;
  uint gotack;
  uint bits;
  int in_lr;
  
  bits = 0;
  switch(groupname) {
  case 0:
  case 1:
  case 0xb:
    gotack = 0;
    break;
  case 2:
  case 6:
    gotack = 2;
    break;
  case 3:
  case 7:
  case 8:
  case 9:
  case 10:
    gotack = 3;
    break;
  case 4:
  case 5:
  case 0xc:
    gotack = 1;
    break;
  default:
    gotack = groupname - 0xd;
    if (4 < gotack) goto LAB_0d401906;
  }
  switch((&switchD_0d401944::switchdataD_0d4042fc)[gotack]) {
  case (undefined *)0xd401946:
    bits = 0xf;
    break;
  case (undefined *)0xd40194a:
    bits = 8;
    break;
  case (undefined *)0xd40194e:
    bits = 4;
    break;
  case (undefined *)0xd401952:
    bits = 2;
    break;
  case (undefined *)0xd401956:
    bits = 1;
  }
LAB_0d401906:
  _i = (undefined2)bits;
  gotack = 0;
  do {
    uVar1 = busRdMEMReg(0x22a);
    if (((bits ^ uVar1) & 0xffff) == 0) {
      _i = 0;
      return in_lr;
    }
    udelay(1);
    gotack = gotack + 1;
  } while (gotack < 1000000);
  _i = 0;
  printf("ERROR: ahbMemFlush(0x%x) did not ack!\n",bits);
  return in_lr;
}



undefined8 busRdAHMReg(uint param_1)

{
  undefined4 in_lr;
  undefined8 uVar1;
  
  uVar1 = _busRdAHMReg(param_1);
  rdbf(0);
  return CONCAT44(in_lr,(int)uVar1);
}



uint ahbMemRBInvalidate(int buffer)

{
  uint in_lr;
  
  rdbf(buffer);
  if (buffer != 0) {
    rdbf(0);
  }
  return in_lr;
}



// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void gpioEnableIOPInt(uint param_1)

{
  _DAT_0d8000f4 = _DAT_0d8000f4 | param_1;
  return;
}



// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void gpioSetIOPIntPol(uint param_1)

{
  _DAT_0d8000ec = _DAT_0d8000ec | param_1;
  return;
}



// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void gpioSetOwner(undefined4 param_1)

{
  _DAT_0d8000fc = param_1;
  return;
}



// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void gpioSet(uint param_1,uint param_2)

{
  _DAT_0d8000e0 = _DAT_0d8000e0 & ~param_1 | param_2;
  return;
}



// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void gpioSetOutputEnable(undefined4 param_1)

{
  _DAT_0d8000e4 = param_1;
  return;
}



// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void gpioPPCSet(uint param_1,uint param_2)

{
  _DAT_0d8000c0 = _DAT_0d8000c0 & ~param_1 | param_2;
  return;
}



// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void gpioPPCSetOutputEnable(undefined4 param_1)

{
  _DAT_0d8000c4 = param_1;
  return;
}



// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void gpioEnable(undefined4 param_1)

{
  _DAT_0d8000dc = param_1;
  return;
}



// WARNING: Globals starting with '_' overlap smaller symbols at the same address

int initGPIO(void)

{
  int in_lr;
  
  gpioSetOwner(_DAT_0d8000fc & 0xfffffff0 | 0xc3a0);
  gpioSet(0xff1c1e,0xc);
  gpioSetOutputEnable(_DAT_0d8000e4 | 0xff1c1e);
  gpioPPCSet(0xc320,0);
  gpioPPCSetOutputEnable(_DAT_0d8000c4 | 0xc320);
  gpioEnable(_DAT_0d8000dc | 0xffffff);
  udelay(2000);
  gpioEnableIOPInt(1);
  gpioSetIOPIntPol(1);
  return in_lr;
}



int outputLed(int value)

{
  int in_lr;
  
  gpioSet(0xff0000,(uint)(value << 0x18) >> 8);
  return in_lr;
}



// WARNING: Globals starting with '_' overlap smaller symbols at the same address

undefined4 gpioCompatSwitch(void)

{
  undefined4 in_lr;
  
  gpioSet(0xc120,_DAT_0d8000c0);
  gpioSetOwner(0);
  gpioSetOutputEnable(_DAT_0d8000e4 | 0xffdf3e);
  return in_lr;
}



// WARNING: Removing unreachable block (ram,0x0d4024a2)
// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void __init_DDR_Napa(int param_1,uint param_2,undefined4 param_3,char param_4,char param_5)

{
  uint uVar1;
  uint uVar2;
  undefined1 *puVar3;
  uint uVar4;
  uint uVar5;
  uint *puVar6;
  int iVar7;
  undefined4 unaff_r8;
  int iVar8;
  int iVar9;
  code *UNRECOVERED_JUMPTABLE;
  undefined8 uVar10;
  uint uStack_34;
  uint uStack_30;
  uint uStack_2c;
  uint uStack_28;
  
  uVar1 = param_2 & 0xffff;
  getChipId(&uStack_28,&uStack_2c);
  if ((_DAT_0d800194 & 0x800) != 0) {
    FUN_0d40246c();
  }
  _DAT_0d800194 = _DAT_0d800194 & 0xfffff7ff;
  udelay(100);
  _DAT_0d800180 = _DAT_0d800180 & 0xffffffdf;
  _DAT_0d800194 = _DAT_0d800194 & 0xfffffeff;
  udelay(10);
  _DAT_0d800194 = _DAT_0d800194 & 0xfffff7ff | 0x800;
  udelay(5);
  _DAT_0d800194 = _DAT_0d800194 & 0xfffffeff | 0x100;
  udelay(10);
  _DAT_0d800194 = _DAT_0d800194 & 0xfffffeff;
  uVar5 = _DAT_0d800194 | 0x100;
  udelay(100);
  _DAT_0d800194 = uVar5;
  udelay(5);
  getChipId(&uStack_30,&uStack_34);
  uVar5 = __udivsi3(uVar1,0x1b);
  uVar5 = uVar5 & 0xffff;
  if (uStack_30 < 2) {
    uVar2 = 2000;
    if (uVar5 * 0x1b - uVar1 == 0) goto LAB_0d401be8;
LAB_0d401bc4:
    if (uVar2 >> 1 < uVar1) {
      iVar9 = 1;
      iVar8 = 0;
      uVar2 = 0x1a;
      iVar7 = uVar1 - 1;
    }
    else {
      iVar9 = 0;
      iVar8 = 4;
      uVar2 = 0x1a;
      iVar7 = uVar1 * 2 + -1;
    }
  }
  else {
    uVar2 = 0x640;
    if (uVar5 * 0x1b - uVar1 != 0) goto LAB_0d401bc4;
LAB_0d401be8:
    if (uVar1 <= uVar2 >> 1) {
      uVar5 = FUN_0d4024d8(uVar5);
    }
    iVar9 = 1;
    iVar8 = 0;
    uVar2 = 0;
    iVar7 = uVar5 - 1;
  }
  _DAT_0d8001c0 = _DAT_0d8001c0 & 0x3fffffff;
  udelay(100);
  _DAT_0d8001bc =
       (_DAT_0d8001bc & 0xfffc0000 | uVar2 | iVar7 << 6) & 0xe803ffff | iVar8 << 0x12 |
       iVar9 << 0x1c;
  udelay(100);
  _DAT_0d8001c0 = _DAT_0d8001c0 & 0x27ffffff | 0x58000000;
  udelay(1000);
  _DAT_0d8001c0 = _DAT_0d8001c0 & 0x7fffffff | 0x80000000;
  udelay(1000);
  writeDDRMemReg(0x100,0x24);
  udelay(5);
  writeDDRMemReg(0x100,0x20);
  writeSeqMemReg(0x4b,0);
  if (param_5 == '\x01') {
    writeSeqMemReg(0x48,0xd09);
    udelay(0x32);
  }
  else {
    writeSeqMemReg(0x48,0xd0b);
    udelay(0x32);
    if (param_5 != '\x01') {
      uVar1 = 0x50b;
      goto LAB_0d401cea;
    }
  }
  uVar1 = 0x509;
LAB_0d401cea:
  writeSeqMemReg(0x48,uVar1);
  udelay(0x32);
  if (uStack_28 < 2) {
    writeSeqMemReg(0x3e,0xf0f0);
    writeSeqMemReg(0x3f,0xf0f0);
    writeSeqMemReg(0x40,0x1616);
    writeSeqMemReg(0x41,0x1616);
    writeSeqMemReg(0x42,0x1616);
    writeSeqMemReg(0x43,0x1616);
    udelay(0x32);
  }
  else {
    writeSeqMemReg(0x3e,0x7676);
    writeSeqMemReg(0x3f,0x7676);
    writeSeqMemReg(0x40,0);
    writeSeqMemReg(0x41,0);
    writeSeqMemReg(0x42,0);
    writeSeqMemReg(0x43,0);
    udelay(0x32);
  }
  if (param_5 == '\x01') {
    uVar1 = 0x109;
  }
  else {
    uVar1 = 0x10b;
  }
  writeSeqMemReg(0x48,uVar1);
  udelay(10);
  writeSeqMemReg(0x47,0x8000);
  writeSeqMemReg(0x27,0);
  writeDDRMemReg(0x10c,0x1ff);
  writeDDRMemReg(0x10d,0xfff);
  writeDDRMemReg(0x10e,7);
  writeDDRMemReg(0x10b,1);
  writeDDRMemReg(0x109,4);
  writeDDRMemReg(0x108,6);
  writeDDRMemReg(0x10a,2);
  writeDDRMemReg(0x15b,0xeff);
  writeDDRMemReg(0x134,8);
  writeDDRMemReg(0x135,0xc);
  writeDDRMemReg(0x136,0x18);
  writeDDRMemReg(0x140,6);
  writeDDRMemReg(0x15a,5);
  writeDDRMemReg(0x137,5);
  writeDDRMemReg(0x138,5);
  writeDDRMemReg(0x139,5);
  writeDDRMemReg(0x13a,5);
  writeDDRMemReg(0x13b,5);
  writeDDRMemReg(0x13c,5);
  writeDDRMemReg(0x13d,5);
  writeDDRMemReg(0x13e,5);
  writeDDRMemReg(0x13f,5);
  writeSeqMemReg(0x1c,0);
  writeSeqMemReg(0x1b,0);
  writeSeqMemReg(0,0);
  writeSeqMemReg(0x15,1);
  writeSeqMemReg(0x16,0);
  writeSeqMemReg(0x25,1);
  writeSeqMemReg(0x10,0);
  writeSeqMemReg(0x23,8);
  writeSeqMemReg(1,7);
  writeSeqMemReg(2,4);
  writeSeqMemReg(5,7);
  writeSeqMemReg(8,4);
  writeSeqMemReg(9,0x18);
  writeSeqMemReg(10,0x1b);
  writeSeqMemReg(4,0x17);
  writeSeqMemReg(0x21,0xb);
  writeSeqMemReg(0xb,9);
  writeSeqMemReg(0xc,0xb);
  writeSeqMemReg(0xd,6);
  writeSeqMemReg(0xe,0xc);
  writeSeqMemReg(0xf,0x17);
  writeSeqMemReg(0x11,0xfc00);
  writeSeqMemReg(0x12,0x1f);
  writeSeqMemReg(0x13,0);
  writeSeqMemReg(0x14,0);
  writeSeqMemReg(6,2);
  writeSeqMemReg(7,10);
  writeSeqMemReg(0x22,8);
  writeSeqMemReg(0x1f,0x1fe0);
  writeSeqMemReg(0x20,0);
  if (uStack_28 < 2) {
    writeSeqMemReg(0x2c,0x7252);
    writeSeqMemReg(0x2d,0x4a5e);
    writeSeqMemReg(0x2e,0x7bde);
    uVar1 = 0xde;
  }
  else {
    writeSeqMemReg(0x2c,0x4210);
    writeSeqMemReg(0x2d,0x4210);
    writeSeqMemReg(0x2e,0x4210);
    uVar1 = 0xd0;
  }
  writeSeqMemReg(0x2f,uVar1);
  writeSeqMemReg(0x30,0xcc);
  writeSeqMemReg(0x31,0);
  writeSeqMemReg(0x32,0xcc);
  writeSeqMemReg(0x33,0);
  writeSeqMemReg(0x34,0xcc);
  writeSeqMemReg(0x35,0);
  writeSeqMemReg(0x36,0x8ec);
  writeSeqMemReg(0x37,0);
  writeSeqMemReg(0x38,0x476);
  writeSeqMemReg(0x39,0);
  if (param_5 == '\x01') {
    writeSeqMemReg(0x3a,0x800f);
    writeSeqMemReg(0x3b,7);
    writeSeqMemReg(0x3c,0x800f);
    uVar1 = 7;
  }
  else {
    writeSeqMemReg(0x3a,0);
    writeSeqMemReg(0x3b,0);
    writeSeqMemReg(0x3c,0);
    uVar1 = 0;
  }
  writeSeqMemReg(0x3d,uVar1);
  uVar10 = getRsvdSRAMStart();
  iVar7 = (int)uVar10;
  uVar10 = readSeqMemReg(0x3a);
  *(int *)(iVar7 + 4) = (int)uVar10;
  uVar10 = readSeqMemReg(0x3b);
  *(int *)(iVar7 + 8) = (int)uVar10;
  uVar10 = readSeqMemReg(0x3c);
  *(int *)(iVar7 + 0xc) = (int)uVar10;
  uVar10 = readSeqMemReg(0x3d);
  *(int *)(iVar7 + 0x10) = (int)uVar10;
  *(undefined4 *)(iVar7 + 0x14) = 0xcafebabe;
  writeSeqMemReg(0x45,0);
  writeDDRMemReg(0x100,0);
  udelay(5);
  writeSeqMemReg(0x18,1);
  udelay(5);
  writeSeqMemReg(0x17,1);
  udelay(200);
  writeSeqMemReg(0x4b,0);
  writeSeqMemReg(0x4c,1);
  writeDDRMemReg(0x160,0xffff);
  writeDDRMemReg(0x161,0x20);
  writeDDRMemReg(0x161,0x21);
  writeDDRMemReg(0x161,0x20);
  udelay(1);
  writeDDRMemReg(0x160,0x2882);
  writeDDRMemReg(0x161,0x22);
  writeDDRMemReg(0x161,0x23);
  writeDDRMemReg(0x161,0x22);
  udelay(5);
  writeDDRMemReg(0x160,0x2882);
  writeDDRMemReg(0x161,0x24);
  writeDDRMemReg(0x161,0x25);
  writeDDRMemReg(0x161,0x24);
  udelay(5);
  writeDDRMemReg(0x160,0x2c82);
  writeDDRMemReg(0x161,0x22);
  writeDDRMemReg(0x161,0x23);
  writeDDRMemReg(0x161,0x22);
  udelay(5);
  uVar10 = readSeqMemReg(0x29);
  uVar2 = (uint)uVar10 & 0xff;
  writeDDRMemReg(0x160,0x2882);
  writeDDRMemReg(0x161,0x22);
  writeDDRMemReg(0x161,0x23);
  writeDDRMemReg(0x161,0x22);
  udelay(5);
  writeDDRMemReg(0x160,0x2c82);
  writeDDRMemReg(0x161,0x24);
  writeDDRMemReg(0x161,0x25);
  writeDDRMemReg(0x161,0x24);
  udelay(5);
  uVar10 = readSeqMemReg(0x29);
  uVar1 = (uint)((int)uVar10 << 0x10) >> 0x18;
  writeDDRMemReg(0x160,0x2882);
  writeDDRMemReg(0x161,0x24);
  writeDDRMemReg(0x161,0x25);
  writeDDRMemReg(0x161,0x24);
  udelay(5);
  writeDDRMemReg(0x160,0x903);
  writeDDRMemReg(0x161,0x22);
  writeDDRMemReg(0x161,0x23);
  writeDDRMemReg(0x161,0x22);
  udelay(1);
  writeDDRMemReg(0x160,0x903);
  writeDDRMemReg(0x161,0x24);
  writeDDRMemReg(0x161,0x25);
  writeDDRMemReg(0x161,0x24);
  udelay(1);
  writeSeqMemReg(0x4c,0);
  uVar5 = 0;
  puVar3 = DRAM_table;
  iVar7 = 0;
  do {
    if ((*(ushort *)((int)puVar3 + 2) & uVar2) == (uint)*(ushort *)puVar3) goto LAB_0d40224e;
    uVar5 = uVar5 + 1;
    iVar7 = iVar7 + 0x6c;
    puVar3 = (undefined1 *)((int)puVar3 + 0x6c);
  } while (uVar5 < 4);
  iVar7 = uVar5 * 0x6c;
LAB_0d40224e:
  writeSeqMemReg(0x18,0);
  writeSeqMemReg(0x17,0);
  udelay(200);
  if (uVar2 == uVar1) {
    writeSeqMemReg(0x18,(uint)*(ushort *)(DRAM_table + iVar7 + 0x38));
    iVar8 = 0xd404670;
  }
  else {
    writeSeqMemReg(0x18,(uint)*(ushort *)(DRAM_table + iVar7 + 4));
    iVar8 = 0xd40463c;
  }
  puVar6 = (uint *)(iVar7 + iVar8);
  udelay(5);
  writeSeqMemReg(0x17,1);
  udelay(200);
  writeSeqMemReg(0x18,1);
  writeSeqMemReg(0x4b,0);
  if (uVar2 != uVar1) {
    writeDDRMemReg(0x10b,7);
    writeSeqMemReg(0x15,0);
  }
  iVar7 = 0xb;
  do {
    while( true ) {
      uVar5 = *puVar6;
      uVar4 = uVar5 >> 0x10;
      if ((((uVar4 != 0x20) && (uVar4 != 0x22)) && (uVar4 != 0x24)) && (uVar4 != 2)) break;
      writeDDRMemReg(0x160,(uint)*(ushort *)((int)puVar6 + 2));
      writeDDRMemReg(0x161,(uint)*(ushort *)puVar6);
      writeDDRMemReg(0x161,*(ushort *)puVar6 | 1);
      writeDDRMemReg(0x161,(uint)*(ushort *)puVar6);
      udelay(5);
LAB_0d40230c:
      puVar6 = puVar6 + 1;
      iVar7 = iVar7 + -1;
      if (iVar7 < 0) goto LAB_0d402312;
    }
    if (uVar5 >> 0x10 == 0x8000) {
      udelay(uVar5 & 0xffff);
      goto LAB_0d40230c;
    }
    puVar6 = puVar6 + 1;
    iVar7 = iVar7 + -1;
  } while (-1 < iVar7);
LAB_0d402312:
  writeSeqMemReg(0x4c,1);
  writeSeqMemReg(0x4b,0);
  _DAT_0d8b4000 = 0;
  _DAT_0d8b4002 = 0;
  _DAT_0d8b4004 = 0;
  _DAT_0d8b4006 = 0;
  _DAT_0d8b4008 = 0;
  _DAT_0d8b400a = 0;
  _DAT_0d8b400c = 0;
  _DAT_0d8b400e = 0;
  uVar5 = __udivsi3(param_1 * 0xfb0,10000);
  _DAT_0d8b4026 = (short)uVar5 + -1;
  if (param_4 == '\x01') {
    initARAMEmulation();
  }
  else {
    writeSeqMemReg(0x18,0);
    writeSeqMemReg(0x19,1);
    uVar5 = __udivsi3(param_1 * 0x27,10);
    writeDDRMemReg(0x113,uVar5);
  }
  writeDDRMemReg(0x165,0x29);
  writeDDRMemReg(0x164,uVar2);
  writeDDRMemReg(0x165,0x2b);
  writeDDRMemReg(0x164,uVar1);
  uVar10 = getRsvdSRAMStart();
  *(int *)uVar10 = uVar1 * 0x100 + uVar2;
                    // WARNING: Could not recover jumptable at 0x0d4023ca. Too many branches
                    // WARNING: Treating indirect jump as call
  (*UNRECOVERED_JUMPTABLE)(UNRECOVERED_JUMPTABLE,(int)((ulonglong)uVar10 >> 0x20),uVar1,unaff_r8);
  return;
}



// WARNING: Removing unreachable block (ram,0x0d4024a2)
// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void FUN_0d401b3e(undefined param_1,undefined param_2,undefined param_3,undefined param_4,
                 undefined param_5,undefined param_6,undefined param_7,undefined param_8,
                 uint param_9)

{
  uint uVar1;
  undefined1 *puVar2;
  uint uVar3;
  uint *puVar4;
  uint uVar5;
  uint *unaff_r7;
  int iVar6;
  uint data;
  uint unaff_r8;
  int iVar7;
  int iVar8;
  uint unaff_r10;
  undefined8 uVar9;
  undefined3 in_stack_00000000;
  undefined3 in_stack_00000004;
  undefined3 in_stack_00000008;
  uint in_stack_00000018;
  undefined4 in_stack_0000001c;
  code *UNRECOVERED_JUMPTABLE;
  
  *unaff_r7 = *unaff_r7 & 0xfffff7ff;
  udelay(100);
  _DAT_0d800180 = _DAT_0d800180 & 0xffffffdf;
  *unaff_r7 = *unaff_r7 & 0xfffffeff;
  udelay(10);
  *unaff_r7 = *unaff_r7 & 0xfffff7ff | unaff_r8;
  udelay(5);
  *unaff_r7 = *unaff_r7 & 0xfffffeff | 0x100;
  udelay(10);
  uVar5 = *unaff_r7;
  *unaff_r7 = uVar5 & 0xfffffeff;
  udelay(100);
  *unaff_r7 = uVar5 & 0xfffffeff | 0x100;
  udelay(5);
  getChipId(&param_9,(uint *)&stack0x0000000c);
  uVar5 = __udivsi3(unaff_r10,0x1b);
  uVar5 = uVar5 & 0xffff;
  if (param_9 < 2) {
    uVar1 = 2000;
    if (uVar5 * 0x1b - unaff_r10 == 0) goto LAB_0d401be8;
LAB_0d401bc4:
    if ((int)(uVar1 >> 1) < (int)unaff_r10) {
      iVar8 = 1;
      iVar7 = 0;
      uVar1 = 0x1a;
      iVar6 = unaff_r10 - 1;
    }
    else {
      iVar8 = 0;
      iVar7 = 4;
      uVar1 = 0x1a;
      iVar6 = unaff_r10 * 2 + -1;
    }
  }
  else {
    uVar1 = 0x640;
    if (uVar5 * 0x1b - unaff_r10 != 0) goto LAB_0d401bc4;
LAB_0d401be8:
    if ((int)unaff_r10 <= (int)(uVar1 >> 1)) {
      uVar5 = FUN_0d4024d8(uVar5);
    }
    iVar8 = 1;
    iVar7 = 0;
    uVar1 = 0;
    iVar6 = uVar5 - 1;
  }
  _DAT_0d8001c0 = _DAT_0d8001c0 & 0x3fffffff;
  udelay(100);
  _DAT_0d8001bc =
       (_DAT_0d8001bc & 0xfffc0000 | uVar1 | iVar6 << 6) & 0xe803ffff | iVar7 << 0x12 |
       iVar8 << 0x1c;
  udelay(100);
  _DAT_0d8001c0 = _DAT_0d8001c0 & 0x27ffffff | 0x58000000;
  udelay(1000);
  _DAT_0d8001c0 = _DAT_0d8001c0 & 0x7fffffff | 0x80000000;
  udelay(1000);
  writeDDRMemReg(0x100,0x24);
  udelay(5);
  writeDDRMemReg(0x100,0x20);
  writeSeqMemReg(0x4b,0);
  if (_in_stack_00000000 == 1) {
    writeSeqMemReg(0x48,0xd09);
    udelay(0x32);
  }
  else {
    writeSeqMemReg(0x48,0xd0b);
    udelay(0x32);
    if (_in_stack_00000000 != 1) {
      uVar5 = 0x50b;
      goto LAB_0d401cea;
    }
  }
  uVar5 = 0x509;
LAB_0d401cea:
  writeSeqMemReg(0x48,uVar5);
  udelay(0x32);
  if (in_stack_00000018 < 2) {
    writeSeqMemReg(0x3e,0xf0f0);
    writeSeqMemReg(0x3f,0xf0f0);
    writeSeqMemReg(0x40,0x1616);
    writeSeqMemReg(0x41,0x1616);
    writeSeqMemReg(0x42,0x1616);
    writeSeqMemReg(0x43,0x1616);
    udelay(0x32);
  }
  else {
    writeSeqMemReg(0x3e,0x7676);
    writeSeqMemReg(0x3f,0x7676);
    writeSeqMemReg(0x40,0);
    writeSeqMemReg(0x41,0);
    writeSeqMemReg(0x42,0);
    writeSeqMemReg(0x43,0);
    udelay(0x32);
  }
  if (_in_stack_00000000 == 1) {
    uVar5 = 0x109;
  }
  else {
    uVar5 = 0x10b;
  }
  writeSeqMemReg(0x48,uVar5);
  udelay(10);
  writeSeqMemReg(0x47,0x8000);
  writeSeqMemReg(0x27,0);
  writeDDRMemReg(0x10c,0x1ff);
  writeDDRMemReg(0x10d,0xfff);
  writeDDRMemReg(0x10e,7);
  writeDDRMemReg(0x10b,1);
  writeDDRMemReg(0x109,4);
  writeDDRMemReg(0x108,6);
  writeDDRMemReg(0x10a,2);
  writeDDRMemReg(0x15b,0xeff);
  writeDDRMemReg(0x134,8);
  writeDDRMemReg(0x135,0xc);
  writeDDRMemReg(0x136,0x18);
  writeDDRMemReg(0x140,6);
  writeDDRMemReg(0x15a,5);
  writeDDRMemReg(0x137,5);
  writeDDRMemReg(0x138,5);
  writeDDRMemReg(0x139,5);
  writeDDRMemReg(0x13a,5);
  writeDDRMemReg(0x13b,5);
  writeDDRMemReg(0x13c,5);
  writeDDRMemReg(0x13d,5);
  writeDDRMemReg(0x13e,5);
  writeDDRMemReg(0x13f,5);
  writeSeqMemReg(0x1c,0);
  writeSeqMemReg(0x1b,0);
  writeSeqMemReg(0,0);
  writeSeqMemReg(0x15,1);
  writeSeqMemReg(0x16,0);
  writeSeqMemReg(0x25,1);
  writeSeqMemReg(0x10,0);
  writeSeqMemReg(0x23,8);
  writeSeqMemReg(1,7);
  writeSeqMemReg(2,4);
  writeSeqMemReg(5,7);
  writeSeqMemReg(8,4);
  writeSeqMemReg(9,0x18);
  writeSeqMemReg(10,0x1b);
  writeSeqMemReg(4,0x17);
  writeSeqMemReg(0x21,0xb);
  writeSeqMemReg(0xb,9);
  writeSeqMemReg(0xc,0xb);
  writeSeqMemReg(0xd,6);
  writeSeqMemReg(0xe,0xc);
  writeSeqMemReg(0xf,0x17);
  writeSeqMemReg(0x11,0xfc00);
  writeSeqMemReg(0x12,0x1f);
  writeSeqMemReg(0x13,0);
  writeSeqMemReg(0x14,0);
  writeSeqMemReg(6,2);
  writeSeqMemReg(7,10);
  writeSeqMemReg(0x22,8);
  writeSeqMemReg(0x1f,0x1fe0);
  writeSeqMemReg(0x20,0);
  if (in_stack_00000018 < 2) {
    writeSeqMemReg(0x2c,0x7252);
    writeSeqMemReg(0x2d,0x4a5e);
    writeSeqMemReg(0x2e,0x7bde);
    uVar5 = 0xde;
  }
  else {
    writeSeqMemReg(0x2c,0x4210);
    writeSeqMemReg(0x2d,0x4210);
    writeSeqMemReg(0x2e,0x4210);
    uVar5 = 0xd0;
  }
  writeSeqMemReg(0x2f,uVar5);
  writeSeqMemReg(0x30,0xcc);
  writeSeqMemReg(0x31,0);
  writeSeqMemReg(0x32,0xcc);
  writeSeqMemReg(0x33,0);
  writeSeqMemReg(0x34,0xcc);
  writeSeqMemReg(0x35,0);
  writeSeqMemReg(0x36,0x8ec);
  writeSeqMemReg(0x37,0);
  writeSeqMemReg(0x38,0x476);
  writeSeqMemReg(0x39,0);
  if (_in_stack_00000000 == 1) {
    writeSeqMemReg(0x3a,0x800f);
    writeSeqMemReg(0x3b,7);
    writeSeqMemReg(0x3c,0x800f);
    uVar5 = 7;
  }
  else {
    writeSeqMemReg(0x3a,0);
    writeSeqMemReg(0x3b,0);
    writeSeqMemReg(0x3c,0);
    uVar5 = 0;
  }
  writeSeqMemReg(0x3d,uVar5);
  uVar9 = getRsvdSRAMStart();
  iVar6 = (int)uVar9;
  uVar9 = readSeqMemReg(0x3a);
  *(int *)(iVar6 + 4) = (int)uVar9;
  uVar9 = readSeqMemReg(0x3b);
  *(int *)(iVar6 + 8) = (int)uVar9;
  uVar9 = readSeqMemReg(0x3c);
  *(int *)(iVar6 + 0xc) = (int)uVar9;
  uVar9 = readSeqMemReg(0x3d);
  *(int *)(iVar6 + 0x10) = (int)uVar9;
  *(undefined4 *)(iVar6 + 0x14) = 0xcafebabe;
  writeSeqMemReg(0x45,0);
  writeDDRMemReg(0x100,0);
  udelay(5);
  writeSeqMemReg(0x18,1);
  udelay(5);
  writeSeqMemReg(0x17,1);
  udelay(200);
  writeSeqMemReg(0x4b,0);
  writeSeqMemReg(0x4c,1);
  writeDDRMemReg(0x160,0xffff);
  writeDDRMemReg(0x161,0x20);
  writeDDRMemReg(0x161,0x21);
  writeDDRMemReg(0x161,0x20);
  udelay(1);
  writeDDRMemReg(0x160,0x2882);
  writeDDRMemReg(0x161,0x22);
  writeDDRMemReg(0x161,0x23);
  writeDDRMemReg(0x161,0x22);
  udelay(5);
  writeDDRMemReg(0x160,0x2882);
  writeDDRMemReg(0x161,0x24);
  writeDDRMemReg(0x161,0x25);
  writeDDRMemReg(0x161,0x24);
  udelay(5);
  writeDDRMemReg(0x160,0x2c82);
  writeDDRMemReg(0x161,0x22);
  writeDDRMemReg(0x161,0x23);
  writeDDRMemReg(0x161,0x22);
  udelay(5);
  uVar9 = readSeqMemReg(0x29);
  data = (uint)uVar9 & 0xff;
  writeDDRMemReg(0x160,0x2882);
  writeDDRMemReg(0x161,0x22);
  writeDDRMemReg(0x161,0x23);
  writeDDRMemReg(0x161,0x22);
  udelay(5);
  writeDDRMemReg(0x160,0x2c82);
  writeDDRMemReg(0x161,0x24);
  writeDDRMemReg(0x161,0x25);
  writeDDRMemReg(0x161,0x24);
  udelay(5);
  uVar9 = readSeqMemReg(0x29);
  uVar5 = (uint)((int)uVar9 << 0x10) >> 0x18;
  writeDDRMemReg(0x160,0x2882);
  writeDDRMemReg(0x161,0x24);
  writeDDRMemReg(0x161,0x25);
  writeDDRMemReg(0x161,0x24);
  udelay(5);
  writeDDRMemReg(0x160,0x903);
  writeDDRMemReg(0x161,0x22);
  writeDDRMemReg(0x161,0x23);
  writeDDRMemReg(0x161,0x22);
  udelay(1);
  writeDDRMemReg(0x160,0x903);
  writeDDRMemReg(0x161,0x24);
  writeDDRMemReg(0x161,0x25);
  writeDDRMemReg(0x161,0x24);
  udelay(1);
  writeSeqMemReg(0x4c,0);
  uVar1 = 0;
  puVar2 = DRAM_table;
  iVar6 = 0;
  do {
    if ((*(ushort *)((int)puVar2 + 2) & data) == (uint)*(ushort *)puVar2) goto LAB_0d40224e;
    uVar1 = uVar1 + 1;
    iVar6 = iVar6 + 0x6c;
    puVar2 = (undefined1 *)((int)puVar2 + 0x6c);
  } while (uVar1 < 4);
  iVar6 = uVar1 * 0x6c;
LAB_0d40224e:
  writeSeqMemReg(0x18,0);
  writeSeqMemReg(0x17,0);
  udelay(200);
  if (data == uVar5) {
    writeSeqMemReg(0x18,(uint)*(ushort *)(DRAM_table + iVar6 + 0x38));
    iVar7 = 0xd404670;
  }
  else {
    writeSeqMemReg(0x18,(uint)*(ushort *)(DRAM_table + iVar6 + 4));
    iVar7 = 0xd40463c;
  }
  puVar4 = (uint *)(iVar6 + iVar7);
  udelay(5);
  writeSeqMemReg(0x17,1);
  udelay(200);
  writeSeqMemReg(0x18,1);
  writeSeqMemReg(0x4b,0);
  if (data != uVar5) {
    writeDDRMemReg(0x10b,7);
    writeSeqMemReg(0x15,0);
  }
  iVar6 = 0xb;
  do {
    while( true ) {
      uVar1 = *puVar4;
      uVar3 = uVar1 >> 0x10;
      if ((((uVar3 != 0x20) && (uVar3 != 0x22)) && (uVar3 != 0x24)) && (uVar3 != 2)) break;
      writeDDRMemReg(0x160,(uint)*(ushort *)((int)puVar4 + 2));
      writeDDRMemReg(0x161,(uint)*(ushort *)puVar4);
      writeDDRMemReg(0x161,*(ushort *)puVar4 | 1);
      writeDDRMemReg(0x161,(uint)*(ushort *)puVar4);
      udelay(5);
LAB_0d40230c:
      puVar4 = puVar4 + 1;
      iVar6 = iVar6 + -1;
      if (iVar6 < 0) goto LAB_0d402312;
    }
    if (uVar1 >> 0x10 == 0x8000) {
      udelay(uVar1 & 0xffff);
      goto LAB_0d40230c;
    }
    puVar4 = puVar4 + 1;
    iVar6 = iVar6 + -1;
  } while (-1 < iVar6);
LAB_0d402312:
  writeSeqMemReg(0x4c,1);
  writeSeqMemReg(0x4b,0);
  _DAT_0d8b4000 = 0;
  _DAT_0d8b4002 = 0;
  _DAT_0d8b4004 = 0;
  _DAT_0d8b4006 = 0;
  _DAT_0d8b4008 = 0;
  _DAT_0d8b400a = 0;
  _DAT_0d8b400c = 0;
  _DAT_0d8b400e = 0;
  uVar1 = __udivsi3(_in_stack_00000008 * 0xfb0,10000);
  _DAT_0d8b4026 = (short)uVar1 + -1;
  if (_in_stack_00000004 == 1) {
    initARAMEmulation();
  }
  else {
    writeSeqMemReg(0x18,0);
    writeSeqMemReg(0x19,1);
    uVar1 = __udivsi3(_in_stack_00000008 * 0x27,10);
    writeDDRMemReg(0x113,uVar1);
  }
  writeDDRMemReg(0x165,0x29);
  writeDDRMemReg(0x164,data);
  writeDDRMemReg(0x165,0x2b);
  writeDDRMemReg(0x164,uVar5);
  uVar9 = getRsvdSRAMStart();
  *(int *)uVar9 = uVar5 * 0x100 + data;
                    // WARNING: Could not recover jumptable at 0x0d4023ca. Too many branches
                    // WARNING: Treating indirect jump as call
  (*UNRECOVERED_JUMPTABLE)
            (UNRECOVERED_JUMPTABLE,(int)((ulonglong)uVar9 >> 0x20),uVar5,in_stack_0000001c);
  return;
}



// WARNING: Removing unreachable block (ram,0x0d4024a2)
// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void FUN_0d401c1e(int param_1,undefined param_2,undefined param_3,undefined param_4,int param_5,
                 int param_6,int param_7,undefined param_8,undefined param_9,undefined param_10,
                 uint param_11)

{
  uint uVar1;
  uint uVar2;
  undefined1 *puVar3;
  int iVar4;
  uint uVar5;
  int iVar6;
  uint *puVar7;
  uint data;
  int unaff_r8;
  int unaff_r9;
  uint unaff_r11;
  undefined8 uVar8;
  undefined4 in_stack_0000001c;
  code *UNRECOVERED_JUMPTABLE;
  
  _DAT_0d8001c0 = _DAT_0d8001c0 & 0x3fffffff;
  udelay(100);
  _DAT_0d8001bc =
       (((_DAT_0d8001bc & 0xffffffc0 | unaff_r11) & 0xfffc003f | (param_1 * 2 + -1) * 0x40) &
        0xf803ffff | unaff_r8 << 0x12) & 0xefffffff | unaff_r9 << 0x1c;
  udelay(100);
  _DAT_0d8001c0 = _DAT_0d8001c0 & 0x27ffffff | 0x58000000;
  udelay(1000);
  _DAT_0d8001c0 = _DAT_0d8001c0 & 0x7fffffff | 0x80000000;
  udelay(1000);
  writeDDRMemReg(0x100,0x24);
  udelay(5);
  writeDDRMemReg(0x100,0x20);
  writeSeqMemReg(0x4b,0);
  if (param_5 == 1) {
    writeSeqMemReg(0x48,0xd09);
    udelay(0x32);
  }
  else {
    writeSeqMemReg(0x48,0xd0b);
    udelay(0x32);
    if (param_5 != 1) {
      uVar2 = 0x50b;
      goto LAB_0d401cea;
    }
  }
  uVar2 = 0x509;
LAB_0d401cea:
  writeSeqMemReg(0x48,uVar2);
  udelay(0x32);
  if (param_11 < 2) {
    writeSeqMemReg(0x3e,0xf0f0);
    writeSeqMemReg(0x3f,0xf0f0);
    writeSeqMemReg(0x40,0x1616);
    writeSeqMemReg(0x41,0x1616);
    writeSeqMemReg(0x42,0x1616);
    writeSeqMemReg(0x43,0x1616);
    udelay(0x32);
  }
  else {
    writeSeqMemReg(0x3e,0x7676);
    writeSeqMemReg(0x3f,0x7676);
    writeSeqMemReg(0x40,0);
    writeSeqMemReg(0x41,0);
    writeSeqMemReg(0x42,0);
    writeSeqMemReg(0x43,0);
    udelay(0x32);
  }
  if (param_5 == 1) {
    uVar2 = 0x109;
  }
  else {
    uVar2 = 0x10b;
  }
  writeSeqMemReg(0x48,uVar2);
  udelay(10);
  writeSeqMemReg(0x47,0x8000);
  writeSeqMemReg(0x27,0);
  writeDDRMemReg(0x10c,0x1ff);
  writeDDRMemReg(0x10d,0xfff);
  writeDDRMemReg(0x10e,7);
  writeDDRMemReg(0x10b,1);
  writeDDRMemReg(0x109,4);
  writeDDRMemReg(0x108,6);
  writeDDRMemReg(0x10a,2);
  writeDDRMemReg(0x15b,0xeff);
  writeDDRMemReg(0x134,8);
  writeDDRMemReg(0x135,0xc);
  writeDDRMemReg(0x136,0x18);
  writeDDRMemReg(0x140,6);
  writeDDRMemReg(0x15a,5);
  writeDDRMemReg(0x137,5);
  writeDDRMemReg(0x138,5);
  writeDDRMemReg(0x139,5);
  writeDDRMemReg(0x13a,5);
  writeDDRMemReg(0x13b,5);
  writeDDRMemReg(0x13c,5);
  writeDDRMemReg(0x13d,5);
  writeDDRMemReg(0x13e,5);
  writeDDRMemReg(0x13f,5);
  writeSeqMemReg(0x1c,0);
  writeSeqMemReg(0x1b,0);
  writeSeqMemReg(0,0);
  writeSeqMemReg(0x15,1);
  writeSeqMemReg(0x16,0);
  writeSeqMemReg(0x25,1);
  writeSeqMemReg(0x10,0);
  writeSeqMemReg(0x23,8);
  writeSeqMemReg(1,7);
  writeSeqMemReg(2,4);
  writeSeqMemReg(5,7);
  writeSeqMemReg(8,4);
  writeSeqMemReg(9,0x18);
  writeSeqMemReg(10,0x1b);
  writeSeqMemReg(4,0x17);
  writeSeqMemReg(0x21,0xb);
  writeSeqMemReg(0xb,9);
  writeSeqMemReg(0xc,0xb);
  writeSeqMemReg(0xd,6);
  writeSeqMemReg(0xe,0xc);
  writeSeqMemReg(0xf,0x17);
  writeSeqMemReg(0x11,0xfc00);
  writeSeqMemReg(0x12,0x1f);
  writeSeqMemReg(0x13,0);
  writeSeqMemReg(0x14,0);
  writeSeqMemReg(6,2);
  writeSeqMemReg(7,10);
  writeSeqMemReg(0x22,8);
  writeSeqMemReg(0x1f,0x1fe0);
  writeSeqMemReg(0x20,0);
  if (param_11 < 2) {
    writeSeqMemReg(0x2c,0x7252);
    writeSeqMemReg(0x2d,0x4a5e);
    writeSeqMemReg(0x2e,0x7bde);
    uVar2 = 0xde;
  }
  else {
    writeSeqMemReg(0x2c,0x4210);
    writeSeqMemReg(0x2d,0x4210);
    writeSeqMemReg(0x2e,0x4210);
    uVar2 = 0xd0;
  }
  writeSeqMemReg(0x2f,uVar2);
  writeSeqMemReg(0x30,0xcc);
  writeSeqMemReg(0x31,0);
  writeSeqMemReg(0x32,0xcc);
  writeSeqMemReg(0x33,0);
  writeSeqMemReg(0x34,0xcc);
  writeSeqMemReg(0x35,0);
  writeSeqMemReg(0x36,0x8ec);
  writeSeqMemReg(0x37,0);
  writeSeqMemReg(0x38,0x476);
  writeSeqMemReg(0x39,0);
  if (param_5 == 1) {
    writeSeqMemReg(0x3a,0x800f);
    writeSeqMemReg(0x3b,7);
    writeSeqMemReg(0x3c,0x800f);
    uVar2 = 7;
  }
  else {
    writeSeqMemReg(0x3a,0);
    writeSeqMemReg(0x3b,0);
    writeSeqMemReg(0x3c,0);
    uVar2 = 0;
  }
  writeSeqMemReg(0x3d,uVar2);
  uVar8 = getRsvdSRAMStart();
  iVar4 = (int)uVar8;
  uVar8 = readSeqMemReg(0x3a);
  *(int *)(iVar4 + 4) = (int)uVar8;
  uVar8 = readSeqMemReg(0x3b);
  *(int *)(iVar4 + 8) = (int)uVar8;
  uVar8 = readSeqMemReg(0x3c);
  *(int *)(iVar4 + 0xc) = (int)uVar8;
  uVar8 = readSeqMemReg(0x3d);
  *(int *)(iVar4 + 0x10) = (int)uVar8;
  *(undefined4 *)(iVar4 + 0x14) = 0xcafebabe;
  writeSeqMemReg(0x45,0);
  writeDDRMemReg(0x100,0);
  udelay(5);
  writeSeqMemReg(0x18,1);
  udelay(5);
  writeSeqMemReg(0x17,1);
  udelay(200);
  writeSeqMemReg(0x4b,0);
  writeSeqMemReg(0x4c,1);
  writeDDRMemReg(0x160,0xffff);
  writeDDRMemReg(0x161,0x20);
  writeDDRMemReg(0x161,0x21);
  writeDDRMemReg(0x161,0x20);
  udelay(1);
  writeDDRMemReg(0x160,0x2882);
  writeDDRMemReg(0x161,0x22);
  writeDDRMemReg(0x161,0x23);
  writeDDRMemReg(0x161,0x22);
  udelay(5);
  writeDDRMemReg(0x160,0x2882);
  writeDDRMemReg(0x161,0x24);
  writeDDRMemReg(0x161,0x25);
  writeDDRMemReg(0x161,0x24);
  udelay(5);
  writeDDRMemReg(0x160,0x2c82);
  writeDDRMemReg(0x161,0x22);
  writeDDRMemReg(0x161,0x23);
  writeDDRMemReg(0x161,0x22);
  udelay(5);
  uVar8 = readSeqMemReg(0x29);
  data = (uint)uVar8 & 0xff;
  writeDDRMemReg(0x160,0x2882);
  writeDDRMemReg(0x161,0x22);
  writeDDRMemReg(0x161,0x23);
  writeDDRMemReg(0x161,0x22);
  udelay(5);
  writeDDRMemReg(0x160,0x2c82);
  writeDDRMemReg(0x161,0x24);
  writeDDRMemReg(0x161,0x25);
  writeDDRMemReg(0x161,0x24);
  udelay(5);
  uVar8 = readSeqMemReg(0x29);
  uVar2 = (uint)((int)uVar8 << 0x10) >> 0x18;
  writeDDRMemReg(0x160,0x2882);
  writeDDRMemReg(0x161,0x24);
  writeDDRMemReg(0x161,0x25);
  writeDDRMemReg(0x161,0x24);
  udelay(5);
  writeDDRMemReg(0x160,0x903);
  writeDDRMemReg(0x161,0x22);
  writeDDRMemReg(0x161,0x23);
  writeDDRMemReg(0x161,0x22);
  udelay(1);
  writeDDRMemReg(0x160,0x903);
  writeDDRMemReg(0x161,0x24);
  writeDDRMemReg(0x161,0x25);
  writeDDRMemReg(0x161,0x24);
  udelay(1);
  writeSeqMemReg(0x4c,0);
  uVar1 = 0;
  puVar3 = DRAM_table;
  iVar4 = 0;
  do {
    if ((*(ushort *)((int)puVar3 + 2) & data) == (uint)*(ushort *)puVar3) goto LAB_0d40224e;
    uVar1 = uVar1 + 1;
    iVar4 = iVar4 + 0x6c;
    puVar3 = (undefined1 *)((int)puVar3 + 0x6c);
  } while (uVar1 < 4);
  iVar4 = uVar1 * 0x6c;
LAB_0d40224e:
  writeSeqMemReg(0x18,0);
  writeSeqMemReg(0x17,0);
  udelay(200);
  if (data == uVar2) {
    writeSeqMemReg(0x18,(uint)*(ushort *)(DRAM_table + iVar4 + 0x38));
    iVar6 = 0xd404670;
  }
  else {
    writeSeqMemReg(0x18,(uint)*(ushort *)(DRAM_table + iVar4 + 4));
    iVar6 = 0xd40463c;
  }
  puVar7 = (uint *)(iVar4 + iVar6);
  udelay(5);
  writeSeqMemReg(0x17,1);
  udelay(200);
  writeSeqMemReg(0x18,1);
  writeSeqMemReg(0x4b,0);
  if (data != uVar2) {
    writeDDRMemReg(0x10b,7);
    writeSeqMemReg(0x15,0);
  }
  iVar4 = 0xb;
  do {
    while( true ) {
      uVar1 = *puVar7;
      uVar5 = uVar1 >> 0x10;
      if ((((uVar5 != 0x20) && (uVar5 != 0x22)) && (uVar5 != 0x24)) && (uVar5 != 2)) break;
      writeDDRMemReg(0x160,(uint)*(ushort *)((int)puVar7 + 2));
      writeDDRMemReg(0x161,(uint)*(ushort *)puVar7);
      writeDDRMemReg(0x161,*(ushort *)puVar7 | 1);
      writeDDRMemReg(0x161,(uint)*(ushort *)puVar7);
      udelay(5);
LAB_0d40230c:
      puVar7 = puVar7 + 1;
      iVar4 = iVar4 + -1;
      if (iVar4 < 0) goto LAB_0d402312;
    }
    if (uVar1 >> 0x10 == 0x8000) {
      udelay(uVar1 & 0xffff);
      goto LAB_0d40230c;
    }
    puVar7 = puVar7 + 1;
    iVar4 = iVar4 + -1;
  } while (-1 < iVar4);
LAB_0d402312:
  writeSeqMemReg(0x4c,1);
  writeSeqMemReg(0x4b,0);
  _DAT_0d8b4000 = 0;
  _DAT_0d8b4002 = 0;
  _DAT_0d8b4004 = 0;
  _DAT_0d8b4006 = 0;
  _DAT_0d8b4008 = 0;
  _DAT_0d8b400a = 0;
  _DAT_0d8b400c = 0;
  _DAT_0d8b400e = 0;
  uVar1 = __udivsi3(param_7 * 0xfb0,10000);
  _DAT_0d8b4026 = (short)uVar1 + -1;
  if (param_6 == 1) {
    initARAMEmulation();
  }
  else {
    writeSeqMemReg(0x18,0);
    writeSeqMemReg(0x19,1);
    uVar1 = __udivsi3(param_7 * 0x27,10);
    writeDDRMemReg(0x113,uVar1);
  }
  writeDDRMemReg(0x165,0x29);
  writeDDRMemReg(0x164,data);
  writeDDRMemReg(0x165,0x2b);
  writeDDRMemReg(0x164,uVar2);
  uVar8 = getRsvdSRAMStart();
  *(int *)uVar8 = uVar2 * 0x100 + data;
                    // WARNING: Could not recover jumptable at 0x0d4023ca. Too many branches
                    // WARNING: Treating indirect jump as call
  (*UNRECOVERED_JUMPTABLE)
            (UNRECOVERED_JUMPTABLE,(int)((ulonglong)uVar8 >> 0x20),uVar2,in_stack_0000001c);
  return;
}



// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void FUN_0d40246c(void)

{
  uint uVar1;
  int iVar2;
  uint uVar3;
  undefined1 *puVar4;
  undefined extraout_r1;
  undefined in_r2;
  uint uVar5;
  undefined in_r3;
  int iVar6;
  uint *puVar7;
  uint data;
  undefined8 uVar8;
  int in_stack_00000000;
  undefined uVar9;
  int in_stack_00000004;
  undefined uVar10;
  int in_stack_00000008;
  undefined uVar11;
  undefined in_stack_0000000f;
  undefined4 in_stack_00000010;
  uint in_stack_00000018;
  undefined4 in_stack_0000001c;
  code *UNRECOVERED_JUMPTABLE;
  
  writeSeqMemReg(0x18,0);
  writeSeqMemReg(0x19,0);
  writeSeqMemReg(0x17,0);
  iVar2 = udelay(10);
  FUN_0d401b3e((char)iVar2,extraout_r1,in_r2,in_r3,uVar9,uVar10,uVar11,in_stack_0000000f,
               in_stack_00000010);
  writeSeqMemReg(0x48,0xd09);
  udelay(0x32);
  if (in_stack_00000000 == 1) {
    uVar3 = 0x509;
  }
  else {
    uVar3 = 0x50b;
  }
  writeSeqMemReg(0x48,uVar3);
  udelay(0x32);
  if (in_stack_00000018 < 2) {
    writeSeqMemReg(0x3e,0xf0f0);
    writeSeqMemReg(0x3f,0xf0f0);
    writeSeqMemReg(0x40,0x1616);
    writeSeqMemReg(0x41,0x1616);
    writeSeqMemReg(0x42,0x1616);
    writeSeqMemReg(0x43,0x1616);
    udelay(0x32);
  }
  else {
    writeSeqMemReg(0x3e,0x7676);
    writeSeqMemReg(0x3f,0x7676);
    writeSeqMemReg(0x40,0);
    writeSeqMemReg(0x41,0);
    writeSeqMemReg(0x42,0);
    writeSeqMemReg(0x43,0);
    udelay(0x32);
  }
  if (in_stack_00000000 == 1) {
    uVar3 = 0x109;
  }
  else {
    uVar3 = 0x10b;
  }
  writeSeqMemReg(0x48,uVar3);
  udelay(10);
  writeSeqMemReg(0x47,0x8000);
  writeSeqMemReg(0x27,0);
  writeDDRMemReg(0x10c,0x1ff);
  writeDDRMemReg(0x10d,0xfff);
  writeDDRMemReg(0x10e,7);
  writeDDRMemReg(0x10b,1);
  writeDDRMemReg(0x109,4);
  writeDDRMemReg(0x108,6);
  writeDDRMemReg(0x10a,2);
  writeDDRMemReg(0x15b,0xeff);
  writeDDRMemReg(0x134,8);
  writeDDRMemReg(0x135,0xc);
  writeDDRMemReg(0x136,0x18);
  writeDDRMemReg(0x140,6);
  writeDDRMemReg(0x15a,5);
  writeDDRMemReg(0x137,5);
  writeDDRMemReg(0x138,5);
  writeDDRMemReg(0x139,5);
  writeDDRMemReg(0x13a,5);
  writeDDRMemReg(0x13b,5);
  writeDDRMemReg(0x13c,5);
  writeDDRMemReg(0x13d,5);
  writeDDRMemReg(0x13e,5);
  writeDDRMemReg(0x13f,5);
  writeSeqMemReg(0x1c,0);
  writeSeqMemReg(0x1b,0);
  writeSeqMemReg(0,0);
  writeSeqMemReg(0x15,1);
  writeSeqMemReg(0x16,0);
  writeSeqMemReg(0x25,1);
  writeSeqMemReg(0x10,0);
  writeSeqMemReg(0x23,8);
  writeSeqMemReg(1,7);
  writeSeqMemReg(2,4);
  writeSeqMemReg(5,7);
  writeSeqMemReg(8,4);
  writeSeqMemReg(9,0x18);
  writeSeqMemReg(10,0x1b);
  writeSeqMemReg(4,0x17);
  writeSeqMemReg(0x21,0xb);
  writeSeqMemReg(0xb,9);
  writeSeqMemReg(0xc,0xb);
  writeSeqMemReg(0xd,6);
  writeSeqMemReg(0xe,0xc);
  writeSeqMemReg(0xf,0x17);
  writeSeqMemReg(0x11,0xfc00);
  writeSeqMemReg(0x12,0x1f);
  writeSeqMemReg(0x13,0);
  writeSeqMemReg(0x14,0);
  writeSeqMemReg(6,2);
  writeSeqMemReg(7,10);
  writeSeqMemReg(0x22,8);
  writeSeqMemReg(0x1f,0x1fe0);
  writeSeqMemReg(0x20,0);
  if (in_stack_00000018 < 2) {
    writeSeqMemReg(0x2c,0x7252);
    writeSeqMemReg(0x2d,0x4a5e);
    writeSeqMemReg(0x2e,0x7bde);
    uVar3 = 0xde;
  }
  else {
    writeSeqMemReg(0x2c,0x4210);
    writeSeqMemReg(0x2d,0x4210);
    writeSeqMemReg(0x2e,0x4210);
    uVar3 = 0xd0;
  }
  writeSeqMemReg(0x2f,uVar3);
  writeSeqMemReg(0x30,0xcc);
  writeSeqMemReg(0x31,0);
  writeSeqMemReg(0x32,0xcc);
  writeSeqMemReg(0x33,0);
  writeSeqMemReg(0x34,0xcc);
  writeSeqMemReg(0x35,0);
  writeSeqMemReg(0x36,0x8ec);
  writeSeqMemReg(0x37,0);
  writeSeqMemReg(0x38,0x476);
  writeSeqMemReg(0x39,0);
  if (in_stack_00000000 == 1) {
    writeSeqMemReg(0x3a,0x800f);
    writeSeqMemReg(0x3b,7);
    writeSeqMemReg(0x3c,0x800f);
    uVar3 = 7;
  }
  else {
    writeSeqMemReg(0x3a,0);
    writeSeqMemReg(0x3b,0);
    writeSeqMemReg(0x3c,0);
    uVar3 = 0;
  }
  writeSeqMemReg(0x3d,uVar3);
  uVar8 = getRsvdSRAMStart();
  iVar2 = (int)uVar8;
  uVar8 = readSeqMemReg(0x3a);
  *(int *)(iVar2 + 4) = (int)uVar8;
  uVar8 = readSeqMemReg(0x3b);
  *(int *)(iVar2 + 8) = (int)uVar8;
  uVar8 = readSeqMemReg(0x3c);
  *(int *)(iVar2 + 0xc) = (int)uVar8;
  uVar8 = readSeqMemReg(0x3d);
  *(int *)(iVar2 + 0x10) = (int)uVar8;
  *(undefined4 *)(iVar2 + 0x14) = 0xcafebabe;
  writeSeqMemReg(0x45,0);
  writeDDRMemReg(0x100,0);
  udelay(5);
  writeSeqMemReg(0x18,1);
  udelay(5);
  writeSeqMemReg(0x17,1);
  udelay(200);
  writeSeqMemReg(0x4b,0);
  writeSeqMemReg(0x4c,1);
  writeDDRMemReg(0x160,0xffff);
  writeDDRMemReg(0x161,0x20);
  writeDDRMemReg(0x161,0x21);
  writeDDRMemReg(0x161,0x20);
  udelay(1);
  writeDDRMemReg(0x160,0x2882);
  writeDDRMemReg(0x161,0x22);
  writeDDRMemReg(0x161,0x23);
  writeDDRMemReg(0x161,0x22);
  udelay(5);
  writeDDRMemReg(0x160,0x2882);
  writeDDRMemReg(0x161,0x24);
  writeDDRMemReg(0x161,0x25);
  writeDDRMemReg(0x161,0x24);
  udelay(5);
  writeDDRMemReg(0x160,0x2c82);
  writeDDRMemReg(0x161,0x22);
  writeDDRMemReg(0x161,0x23);
  writeDDRMemReg(0x161,0x22);
  udelay(5);
  uVar8 = readSeqMemReg(0x29);
  data = (uint)uVar8 & 0xff;
  writeDDRMemReg(0x160,0x2882);
  writeDDRMemReg(0x161,0x22);
  writeDDRMemReg(0x161,0x23);
  writeDDRMemReg(0x161,0x22);
  udelay(5);
  writeDDRMemReg(0x160,0x2c82);
  writeDDRMemReg(0x161,0x24);
  writeDDRMemReg(0x161,0x25);
  writeDDRMemReg(0x161,0x24);
  udelay(5);
  uVar8 = readSeqMemReg(0x29);
  uVar3 = (uint)((int)uVar8 << 0x10) >> 0x18;
  writeDDRMemReg(0x160,0x2882);
  writeDDRMemReg(0x161,0x24);
  writeDDRMemReg(0x161,0x25);
  writeDDRMemReg(0x161,0x24);
  udelay(5);
  writeDDRMemReg(0x160,0x903);
  writeDDRMemReg(0x161,0x22);
  writeDDRMemReg(0x161,0x23);
  writeDDRMemReg(0x161,0x22);
  udelay(1);
  writeDDRMemReg(0x160,0x903);
  writeDDRMemReg(0x161,0x24);
  writeDDRMemReg(0x161,0x25);
  writeDDRMemReg(0x161,0x24);
  udelay(1);
  writeSeqMemReg(0x4c,0);
  uVar1 = 0;
  puVar4 = DRAM_table;
  iVar2 = 0;
  do {
    if ((*(ushort *)((int)puVar4 + 2) & data) == (uint)*(ushort *)puVar4) goto LAB_0d40224e;
    uVar1 = uVar1 + 1;
    iVar2 = iVar2 + 0x6c;
    puVar4 = (undefined1 *)((int)puVar4 + 0x6c);
  } while (uVar1 < 4);
  iVar2 = uVar1 * 0x6c;
LAB_0d40224e:
  writeSeqMemReg(0x18,0);
  writeSeqMemReg(0x17,0);
  udelay(200);
  if (data == uVar3) {
    writeSeqMemReg(0x18,(uint)*(ushort *)(DRAM_table + iVar2 + 0x38));
    iVar6 = 0xd404670;
  }
  else {
    writeSeqMemReg(0x18,(uint)*(ushort *)(DRAM_table + iVar2 + 4));
    iVar6 = 0xd40463c;
  }
  puVar7 = (uint *)(iVar2 + iVar6);
  udelay(5);
  writeSeqMemReg(0x17,1);
  udelay(200);
  writeSeqMemReg(0x18,1);
  writeSeqMemReg(0x4b,0);
  if (data != uVar3) {
    writeDDRMemReg(0x10b,7);
    writeSeqMemReg(0x15,0);
  }
  iVar2 = 0xb;
  do {
    while( true ) {
      uVar1 = *puVar7;
      uVar5 = uVar1 >> 0x10;
      if ((((uVar5 != 0x20) && (uVar5 != 0x22)) && (uVar5 != 0x24)) && (uVar5 != 2)) break;
      writeDDRMemReg(0x160,(uint)*(ushort *)((int)puVar7 + 2));
      writeDDRMemReg(0x161,(uint)*(ushort *)puVar7);
      writeDDRMemReg(0x161,*(ushort *)puVar7 | 1);
      writeDDRMemReg(0x161,(uint)*(ushort *)puVar7);
      udelay(5);
LAB_0d40230c:
      puVar7 = puVar7 + 1;
      iVar2 = iVar2 + -1;
      if (iVar2 < 0) goto LAB_0d402312;
    }
    if (uVar1 >> 0x10 == 0x8000) {
      udelay(uVar1 & 0xffff);
      goto LAB_0d40230c;
    }
    puVar7 = puVar7 + 1;
    iVar2 = iVar2 + -1;
  } while (-1 < iVar2);
LAB_0d402312:
  writeSeqMemReg(0x4c,1);
  writeSeqMemReg(0x4b,0);
  _DAT_0d8b4000 = 0;
  _DAT_0d8b4002 = 0;
  _DAT_0d8b4004 = 0;
  _DAT_0d8b4006 = 0;
  _DAT_0d8b4008 = 0;
  _DAT_0d8b400a = 0;
  _DAT_0d8b400c = 0;
  _DAT_0d8b400e = 0;
  uVar1 = __udivsi3(in_stack_00000008 * 0xfb0,10000);
  _DAT_0d8b4026 = (short)uVar1 + -1;
  if (in_stack_00000004 == 1) {
    initARAMEmulation();
  }
  else {
    writeSeqMemReg(0x18,0);
    writeSeqMemReg(0x19,1);
    uVar1 = __udivsi3(in_stack_00000008 * 0x27,10);
    writeDDRMemReg(0x113,uVar1);
  }
  writeDDRMemReg(0x165,0x29);
  writeDDRMemReg(0x164,data);
  writeDDRMemReg(0x165,0x2b);
  writeDDRMemReg(0x164,uVar3);
  uVar8 = getRsvdSRAMStart();
  *(int *)uVar8 = uVar3 * 0x100 + data;
                    // WARNING: Could not recover jumptable at 0x0d4023ca. Too many branches
                    // WARNING: Treating indirect jump as call
  (*UNRECOVERED_JUMPTABLE)
            (UNRECOVERED_JUMPTABLE,(int)((ulonglong)uVar8 >> 0x20),uVar3,in_stack_0000001c);
  return;
}



// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void FUN_0d4024d8(int param_1)

{
  uint uVar1;
  undefined in_r1;
  uint uVar2;
  uint *puVar3;
  uint *unaff_r4;
  int unaff_r5;
  uint unaff_r7;
  undefined8 uVar4;
  undefined4 in_stack_00000000;
  int in_stack_00000004;
  int in_stack_00000008;
  undefined in_stack_0000000f;
  undefined in_stack_00000013;
  undefined in_stack_00000017;
  undefined4 in_stack_00000018;
  undefined4 in_stack_0000001c;
  code *UNRECOVERED_JUMPTABLE;
  
  uVar1 = FUN_0d401c1e(param_1,in_r1,0,4,in_stack_00000000,in_stack_00000004,in_stack_00000008,
                       in_stack_0000000f,in_stack_00000013,in_stack_00000017,in_stack_00000018);
LAB_0d4024e6:
  udelay(uVar1 & 0xffff);
  while (unaff_r5 = unaff_r5 + -1, puVar3 = unaff_r4, -1 < unaff_r5) {
    while( true ) {
      unaff_r4 = puVar3 + 1;
      uVar1 = *unaff_r4;
      uVar2 = uVar1 >> 0x10;
      if ((((uVar2 == 0x20) || (uVar2 == 0x22)) || (uVar2 == 0x24)) || (uVar2 == 2)) break;
      if (uVar1 >> 0x10 == 0x8000) goto LAB_0d4024e6;
      unaff_r5 = unaff_r5 + -1;
      puVar3 = unaff_r4;
      if (unaff_r5 < 0) goto LAB_0d402312;
    }
    writeDDRMemReg(0x160,(uint)*(ushort *)((int)puVar3 + 6));
    writeDDRMemReg(0x161,(uint)*(ushort *)unaff_r4);
    writeDDRMemReg(0x161,*(ushort *)unaff_r4 | 1);
    writeDDRMemReg(0x161,(uint)*(ushort *)unaff_r4);
    udelay(5);
  }
LAB_0d402312:
  writeSeqMemReg(0x4c,1);
  writeSeqMemReg(0x4b,0);
  _DAT_0d8b4000 = 0;
  _DAT_0d8b4002 = 0;
  _DAT_0d8b4004 = 0;
  _DAT_0d8b4006 = 0;
  _DAT_0d8b4008 = 0;
  _DAT_0d8b400a = 0;
  _DAT_0d8b400c = 0;
  _DAT_0d8b400e = 0;
  uVar1 = __udivsi3(in_stack_00000008 * 0xfb0,10000);
  _DAT_0d8b4026 = (short)uVar1 + -1;
  if (in_stack_00000004 == 1) {
    initARAMEmulation();
  }
  else {
    writeSeqMemReg(0x18,0);
    writeSeqMemReg(0x19,1);
    uVar1 = __udivsi3(in_stack_00000008 * 0x27,10);
    writeDDRMemReg(0x113,uVar1);
  }
  writeDDRMemReg(0x165,0x29);
  writeDDRMemReg(0x164,unaff_r7);
  writeDDRMemReg(0x165,0x2b);
  writeDDRMemReg(0x164,4);
  uVar4 = getRsvdSRAMStart();
  *(int *)uVar4 = unaff_r7 + 0x400;
                    // WARNING: Could not recover jumptable at 0x0d4023ca. Too many branches
                    // WARNING: Treating indirect jump as call
  (*UNRECOVERED_JUMPTABLE)
            (UNRECOVERED_JUMPTABLE,(int)((ulonglong)uVar4 >> 0x20),4,in_stack_0000001c);
  return;
}



int init_DDR_Napa(uint compat,char emu)

{
  byte yclk;
  int coreclk;
  int in_lr;
  uint minor;
  uint major;
  
  getChipId(&major,&minor);
  if (major == 0) {
    coreclk = DIStraps();
    yclk = (byte)((uint)(coreclk << 0x1d) >> 0x1f);
  }
  else {
    yclk = 0;
  }
  if ((compat & 0xff) == 1) {
    coreclk = 0xa2;
  }
  else {
    coreclk = 0xf3;
  }
  __init_DDR_Napa(coreclk,999,compat & 0xff,emu,yclk);
  return in_lr;
}



undefined4 set_mem_compat_mode(void)

{
  undefined4 in_lr;
  
  writeDDRMemReg(0x100,1);
  return in_lr;
}



int initARAMEmulation(void)

{
  int in_lr;
  
  writeDDRMemReg(0x10b,7);
  writeSeqMemReg(0x15,0);
  writeSeqMemReg(0x18,1);
  writeSeqMemReg(0x19,0);
  writeSeqMemReg(0x4a,0xe);
  writeSeqMemReg(0xf,8);
  writeSeqMemReg(3,0xe);
  writeSeqMemReg(0x49,0xe);
  udelay(2);
  writeSeqMemReg(0x49,0xf);
  udelay(2);
  return in_lr;
}



void writeDirectMemReg(uint index,uint data)

{
  *(short *)(&DAT_0d8b4000 + index * 2) = (short)data;
  return;
}



uint readDirectMemReg(uint index)

{
  return (uint)*(ushort *)(&DAT_0d8b4000 + index * 2);
}



int writeDDRMemReg(uint index,uint data)

{
  int in_lr;
  
  writeDirectMemReg(0x3a,index);
  readDirectMemReg(0x3a);
  writeDirectMemReg(0x3b,data);
  return in_lr;
}



undefined8 readDDRMemReg(ushort param_1)

{
  uint uVar1;
  undefined4 in_lr;
  
  writeDirectMemReg(0x3a,(uint)param_1);
  readDirectMemReg(0x3a);
  uVar1 = readDirectMemReg(0x3b);
  return CONCAT44(in_lr,uVar1);
}



int writeSeqMemReg(uint index,uint data)

{
  int in_lr;
  
  writeDDRMemReg(0x163,index);
  readDDRMemReg(0x163);
  writeDDRMemReg(0x162,data);
  return in_lr;
}



undefined8 readSeqMemReg(ushort param_1)

{
  undefined4 in_lr;
  undefined8 uVar1;
  
  writeDDRMemReg(0x163,(uint)param_1);
  readDDRMemReg(0x163);
  uVar1 = readDDRMemReg(0x162);
  return CONCAT44(in_lr,(int)uVar1);
}



undefined4 readACRReg(int param_1)

{
  return *(undefined4 *)(param_1 + 0xd800000);
}



void writeACRReg(uint index,uint data)

{
  *(uint *)(index + 0xd800000) = data;
  return;
}



// WARNING: Globals starting with '_' overlap smaller symbols at the same address

undefined4 FUN_0d40268a(void)

{
  uint in_r0;
  uint uVar1;
  undefined4 in_lr;
  
  uVar1 = convertMicrosecondsToTicks(in_r0);
  if (uVar1 < 2) {
    uVar1 = 2;
  }
  uVar1 = _DAT_0d800010 + uVar1;
  if (_DAT_0d800010 < uVar1) {
    do {
    } while (_DAT_0d800010 < uVar1);
  }
  else {
    do {
      do {
      } while ((int)_DAT_0d800010 < 0);
    } while (_DAT_0d800010 < uVar1);
  }
  return in_lr;
}



// WARNING: Globals starting with '_' overlap smaller symbols at the same address

int udelay(uint usec)

{
  uint end;
  int in_lr;
  
  end = convertMicrosecondsToTicks(usec);
  if (end < 2) {
    end = 2;
  }
  end = _DAT_0d800010 + end;
  if (_DAT_0d800010 < end) {
    do {
    } while (_DAT_0d800010 < end);
  }
  else {
    do {
      do {
      } while ((int)_DAT_0d800010 < 0);
    } while (_DAT_0d800010 < end);
  }
  return in_lr;
}



// WARNING: Globals starting with '_' overlap smaller symbols at the same address

undefined4 DIStraps(void)

{
  return _DAT_0d806024;
}



// WARNING: Globals starting with '_' overlap smaller symbols at the same address

undefined8 getRsvdSRAMStart(void)

{
  undefined4 uVar1;
  undefined4 in_lr;
  
  if (_DAT_0d800060 << 0x1a < 0) {
    uVar1 = 0xffffff00;
  }
  else {
    uVar1 = 0xd40ff00;
  }
  return CONCAT44(in_lr,uVar1);
}



uint isSSConfigValid(uint param_1)

{
  return -(param_1 & 0xffff ^ 1) >> 0x1f;
}



// WARNING: Globals starting with '_' overlap smaller symbols at the same address

undefined4 setSS(uint param_1)

{
  uint uVar1;
  ushort *puVar2;
  undefined4 in_lr;
  uint uStack_10;
  uint local_c;
  
  getChipId(&local_c,&uStack_10);
  if ((1 < local_c) && (uVar1 = isSSConfigValid(param_1 & 0xffff), (uVar1 & 0xff) != 0)) {
    if ((param_1 & 0xffff) == 2) {
      puVar2 = &DAT_0d404814;
    }
    else {
      puVar2 = &DAT_0d4047fc;
    }
    _DAT_0d8001b0 =
         ((_DAT_0d8001b0 & 0xffffffc0 | (uint)*puVar2) & 0xfffc003f | (uint)puVar2[1] << 6) &
         0xf803ffff | (uint)puVar2[3] << 0x12;
    _DAT_0d8001b4 =
         (_DAT_0d8001b4 & 0xfffffe00 | (uint)puVar2[4]) & 0xef8001ff | (uint)puVar2[2] << 9 |
         (uint)*(byte *)(puVar2 + 5) << 0x1c;
    _DAT_0d8001a0 =
         ((_DAT_0d8001a0 & 0xfc000000 | *(uint *)(puVar2 + 8)) & 0xfbffffff |
         (uint)*(byte *)(puVar2 + 6) << 0x1a) & 0xf7ffffff |
         (uint)*(byte *)((int)puVar2 + 0xb) << 0x1b;
    _DAT_0d8001a4 =
         (_DAT_0d8001a4 & 0xfffff000 | (uint)puVar2[7]) & 0xff000fff | *(int *)(puVar2 + 10) << 0xc;
    _DAT_0d800194 = _DAT_0d800194 & 0xfffffffe;
  }
  return in_lr;
}



uint convertMicrosecondsToTicks(uint us)

{
  uint GPUClock;
  uint i;
  int j;
  undefined8 ClockFetch;
  
  ClockFetch = getSysClk();
  GPUClock = (uint)ClockFetch;
  if (GPUClock == 0x51) {
    i = us >> 1;
    GPUClock = us >> 3;
  }
  else {
    if (GPUClock < 0x52) {
      if (GPUClock == 0x36) {
        j = (us >> 2) + (us >> 3) + (us >> 5);
        GPUClock = us >> 6;
        goto LAB_0d402816;
      }
    }
    else {
      if (GPUClock == 0x6c) {
        j = (us >> 1) + (us >> 2) + (us >> 4);
        GPUClock = us >> 5;
        goto LAB_0d402816;
      }
                    // GameCube backcompat clock speed (162 MHz)
      if (GPUClock == 0xa2) {
        j = (us >> 2) + us;
        GPUClock = us >> 6;
        goto LAB_0d402816;
      }
    }
    i = (us >> 1) + us + (us >> 2) + (us >> 3);
    GPUClock = us >> 6;
  }
  j = i + GPUClock;
  GPUClock = us >> 7;
LAB_0d402816:
  return j + GPUClock;
}



// WARNING: Globals starting with '_' overlap smaller symbols at the same address

undefined8 getSysClk(void)

{
  uint uVar1;
  undefined4 in_lr;
  uint uStack_10;
  uint local_c;
  
  getChipId(&local_c,&uStack_10);
  if ((int)(_DAT_0d800190 << 0x1e) < 0) {
    uVar1 = 0xa2;
  }
  else {
    if (local_c < 2) {
      if ((_DAT_0d800190 & 1) != 0) {
        uVar1 = __udivsi3(0xf30,_DAT_0d8001b4 & 0x1ff);
        goto LAB_0d402866;
      }
      uVar1 = 0xf30;
    }
    else {
      if ((_DAT_0d800190 & 1) != 0) {
        uVar1 = __udivsi3(0x798,_DAT_0d8001b4 & 0x1ff);
        goto LAB_0d402866;
      }
      uVar1 = 0x798;
    }
    uVar1 = __udivsi3(uVar1,(uint)(_DAT_0d8001b0 << 5) >> 0x17);
  }
LAB_0d402866:
  return CONCAT44(in_lr,uVar1);
}



void * memcpy(void *__dest,void *__src,size_t __n)

{
  undefined4 uVar1;
  uint uVar2;
  undefined4 uVar3;
  undefined4 uVar4;
  undefined4 uVar5;
  uint *puVar6;
  undefined4 uVar7;
  undefined4 uVar8;
  undefined4 uVar9;
  undefined4 uVar10;
  
  if (__n != 0) {
    if ((((uint)__dest | (uint)__src) & 3) == 0) {
      if (0xf < __n) {
        if (0x1f < __n) {
          do {
                    // WARNING: Load size is inaccurate
            uVar1 = *__src;
            uVar3 = *(undefined4 *)((int)__src + 4);
            uVar4 = *(undefined4 *)((int)__src + 8);
            uVar5 = *(undefined4 *)((int)__src + 0xc);
            uVar7 = *(undefined4 *)((int)__src + 0x10);
            uVar8 = *(undefined4 *)((int)__src + 0x14);
            uVar9 = *(undefined4 *)((int)__src + 0x18);
            uVar10 = *(undefined4 *)((int)__src + 0x1c);
            __src = (void *)((int)__src + 0x20);
            *(undefined4 *)__dest = uVar1;
            *(undefined4 *)((int)__dest + 4) = uVar3;
            *(undefined4 *)((int)__dest + 8) = uVar4;
            *(undefined4 *)((int)__dest + 0xc) = uVar5;
            *(undefined4 *)((int)__dest + 0x10) = uVar7;
            *(undefined4 *)((int)__dest + 0x14) = uVar8;
            *(undefined4 *)((int)__dest + 0x18) = uVar9;
            *(undefined4 *)((int)__dest + 0x1c) = uVar10;
            __dest = (void *)((int)__dest + 0x20);
            __n = __n - 0x20;
          } while (0x1f < __n);
        }
        for (; 0xf < __n; __n = __n - 0x10) {
                    // WARNING: Load size is inaccurate
          uVar1 = *__src;
          uVar3 = *(undefined4 *)((int)__src + 4);
          uVar4 = *(undefined4 *)((int)__src + 8);
          uVar5 = *(undefined4 *)((int)__src + 0xc);
          __src = (void *)((int)__src + 0x10);
          *(undefined4 *)__dest = uVar1;
          *(undefined4 *)((int)__dest + 4) = uVar3;
          *(undefined4 *)((int)__dest + 8) = uVar4;
          *(undefined4 *)((int)__dest + 0xc) = uVar5;
          __dest = (void *)((int)__dest + 0x10);
        }
      }
      for (; 3 < __n; __n = __n - 4) {
                    // WARNING: Load size is inaccurate
        *(undefined4 *)__dest = *__src;
        __dest = (undefined4 *)((int)__dest + 4);
        __src = (undefined4 *)((int)__src + 4);
      }
    }
    if (__dest < (undefined4 *)0x1800000) {
      for (; __n != 0; __n = __n - 1) {
        puVar6 = (uint *)((int)__dest - ((uint)__dest & 3));
        uVar2 = ((uint)__dest & 3) * -8 + 0x18;
                    // WARNING: Load size is inaccurate
        *puVar6 = *puVar6 & ~(0xff << (uVar2 & 0xff)) | (uint)*__src << (uVar2 & 0xff);
        __dest = (void *)((int)__dest + 1);
        __src = (undefined4 *)((int)__src + 1);
      }
    }
    else {
      for (; __n != 0; __n = __n - 1) {
                    // WARNING: Load size is inaccurate
        *(byte *)__dest = *__src;
        __dest = (undefined4 *)((int)__dest + 1);
        __src = (undefined4 *)((int)__src + 1);
      }
    }
    return __dest;
  }
  return __dest;
}



undefined8 __ioFlush(int param_1,undefined4 param_2)

{
  undefined4 in_lr;
  undefined8 uVar1;
  
  if (*(int *)(param_1 + 0x18) != 0) {
    *(undefined *)(*(int *)(param_1 + 0x18) + param_1 + 8) = 0;
    *(undefined4 *)(param_1 + 0x18) = 0;
    uVar1 = __sys_write0(param_1 + 8);
    param_2 = (undefined4)((ulonglong)uVar1 >> 0x20);
  }
  return CONCAT44(param_2,in_lr);
}



undefined4 __ioWritec(int param_1,undefined param_2)

{
  int iVar1;
  undefined4 in_lr;
  
  iVar1 = *(int *)(param_1 + 0x18);
  *(undefined *)(iVar1 + param_1 + 8) = param_2;
  iVar1 = iVar1 + 1;
  *(int *)(param_1 + 0x18) = iVar1;
  *(int *)(param_1 + 0x1c) = *(int *)(param_1 + 0x1c) + 1;
  if (iVar1 == 0xf) {
    (**(code **)(param_1 + 4))();
  }
  return in_lr;
}



int vprintf(char *__format,__gnuc_va_list __arg)

{
  code *local_28;
  undefined4 local_24;
  undefined4 local_10;
  int local_c;
  
  local_c = 0;
  local_10 = 0;
  local_28 = __ioWritec + 1;
  local_24 = 0xd4029a9;
  __printf(&local_28,(byte *)__format,(char **)__arg);
  return local_c;
}



int printf(char *__format,...)

{
  int iVar1;
  undefined4 in_r1;
  undefined4 in_r2;
  undefined4 in_r3;
  undefined4 uStack_c;
  undefined4 uStack_8;
  undefined4 uStack_4;
  
  uStack_c = in_r1;
  uStack_8 = in_r2;
  uStack_4 = in_r3;
  iVar1 = vprintf(__format,&uStack_c);
  return iVar1;
}



undefined4 __strFlush(int param_1)

{
  undefined4 in_lr;
  
  if (*(uint *)(param_1 + 0xc) < *(uint *)(param_1 + 0x10)) {
    *(undefined *)(*(int *)(param_1 + 8) + *(uint *)(param_1 + 0xc)) = 0;
  }
  else {
    *(undefined *)(*(int *)(param_1 + 8) + *(int *)(param_1 + 0x10) + -1) = 0;
  }
  return in_lr;
}



undefined4 __strWritec(int param_1,undefined param_2)

{
  undefined4 in_lr;
  
  if (*(uint *)(param_1 + 0xc) < *(uint *)(param_1 + 0x10)) {
    *(undefined *)(*(int *)(param_1 + 8) + *(uint *)(param_1 + 0xc)) = param_2;
  }
  *(int *)(param_1 + 0xc) = *(int *)(param_1 + 0xc) + 1;
  return in_lr;
}



undefined4 __writes(undefined **param_1,char *param_2)

{
  char cVar1;
  undefined4 in_lr;
  
  cVar1 = *param_2;
  while (cVar1 != '\0') {
    (*(code *)*param_1)(param_1,*param_2);
    param_2 = param_2 + 1;
    cVar1 = *param_2;
  }
  return in_lr;
}



// TODO: Find what parameters this function takes

int PAD(undefined **param_1,int param_2,undefined param_3)

{
  int i;
  int in_lr;
  bool j;
  
  i = param_2 + -1;
  if (param_2 != 0) {
    do {
      (*(code *)*param_1)(param_1,param_3);
      j = i != 0;
      i = i + -1;
    } while (j);
  }
  return in_lr;
}



undefined4 __printf(undefined **param_1,byte *param_2,char **param_3)

{
  byte bVar1;
  bool bVar2;
  bool bVar3;
  bool bVar4;
  size_t sVar5;
  int iVar6;
  uint uVar7;
  uint uVar8;
  undefined uVar9;
  char cVar10;
  char *pcVar11;
  char *pcVar12;
  char *pcVar13;
  uint uVar14;
  byte *pbVar15;
  char *pcVar16;
  char cVar17;
  char **ppcVar18;
  undefined4 in_lr;
  bool bVar19;
  char *local_40;
  char local_3c [22];
  char local_26 [2];
  undefined4 uStack_4;
  
  local_40 = (char *)0x0;
  bVar1 = *param_2;
  pbVar15 = param_2 + 1;
  uStack_4 = in_lr;
  do {
    if (bVar1 == 0) {
      (*(code *)param_1[1])(param_1);
      return uStack_4;
    }
    if (bVar1 == 0x25) {
      bVar4 = false;
      bVar2 = false;
      bVar3 = false;
      bVar19 = false;
      pcVar16 = (char *)0x0;
      bVar1 = *pbVar15;
      while( true ) {
        uVar14 = (uint)bVar1;
        pbVar15 = pbVar15 + 1;
        if (uVar14 == 0) break;
        switch(uVar14) {
        case 0x23:
          bVar19 = true;
          break;
        default:
          goto joined_r0x0d402b2a;
        case 0x2a:
          pcVar16 = *param_3;
          param_3 = param_3 + 1;
          break;
        case 0x2b:
          bVar2 = true;
          break;
        case 0x2d:
          bVar3 = true;
          break;
        case 0x30:
          bVar4 = true;
        }
        bVar1 = *pbVar15;
      }
joined_r0x0d402b2a:
      while ((uVar14 - 0x30 & 0xff) < 10) {
        pcVar16 = (char *)((int)pcVar16 * 10 + uVar14 + -0x30);
        bVar1 = *pbVar15;
        pbVar15 = pbVar15 + 1;
        uVar14 = (uint)bVar1;
      }
      iVar6 = 0;
      while (uVar14 == 0x6c) {
        iVar6 = iVar6 + 1;
        bVar1 = *pbVar15;
        pbVar15 = pbVar15 + 1;
        uVar14 = (uint)bVar1;
      }
      switch(uVar14) {
      case 0x58:
      case 0x70:
      case 0x78:
        if (iVar6 == 2) {
          ppcVar18 = param_3 + 2;
          pcVar11 = param_3[1];
          local_40 = *param_3;
        }
        else {
          ppcVar18 = param_3 + 1;
          pcVar11 = *param_3;
        }
        if (uVar14 == 0x70) {
          if (pcVar11 == (char *)0x0) {
            pcVar11 = "(nil)";
            goto LAB_0d402bb8;
          }
          uVar14 = 0x78;
          bVar19 = true;
        }
        if (uVar14 == 0x78) {
          cVar17 = 'W';
        }
        else {
          cVar17 = '7';
        }
        pcVar13 = local_3c;
        bVar2 = true;
        if (iVar6 == 2) {
          uVar8 = 0x3c;
        }
        else {
          uVar8 = 0x1c;
        }
        do {
          if ((int)uVar8 < 0x1d) {
            uVar7 = (uint)pcVar11 >> (uVar8 & 0xff);
          }
          else {
            uVar7 = (uint)local_40 >> (uVar8 - 0x20 & 0xff);
          }
          uVar7 = uVar7 & 0xf;
          if ((uVar7 != 0) || (pcVar12 = pcVar13, !bVar2)) {
            bVar2 = false;
            pcVar12 = pcVar13 + 1;
            if (uVar7 < 10) {
              cVar10 = (char)uVar7 + '0';
            }
            else {
              cVar10 = (char)uVar7 + cVar17;
            }
            *pcVar13 = cVar10;
          }
          uVar8 = uVar8 - 4;
          pcVar13 = pcVar12;
        } while (-1 < (int)uVar8);
        if (bVar2) {
          *pcVar12 = '0';
          pcVar12 = pcVar12 + 1;
        }
        *pcVar12 = '\0';
        sVar5 = strnlen(local_3c,0x18);
        if ((pcVar11 != (char *)0x0) && (bVar19)) {
          sVar5 = sVar5 + 2;
        }
        uVar9 = (undefined)uVar14;
        if (((int)pcVar16 <= (int)sVar5) || (bVar3)) {
          if ((pcVar11 != (char *)0x0) && (bVar19)) {
            PAD(param_1,1,0x30);
            PAD(param_1,1,uVar9);
          }
        }
        else if (bVar4) {
          if ((pcVar11 != (char *)0x0) && (bVar19)) {
            PAD(param_1,1,0x30);
            PAD(param_1,1,uVar9);
          }
          PAD(param_1,(int)pcVar16 - sVar5,0x30);
        }
        else {
          PAD(param_1,(int)pcVar16 - sVar5,0x20);
          if ((pcVar11 != (char *)0x0) && (bVar19)) {
            PAD(param_1,1,0x30);
            PAD(param_1,1,uVar9);
          }
        }
        __writes(param_1,local_3c);
        if (((int)sVar5 < (int)pcVar16) && (bVar3)) {
          PAD(param_1,(int)pcVar16 - sVar5,0x20);
        }
        break;
      default:
        (*(code *)*param_1)(param_1,uVar14);
        ppcVar18 = param_3;
        break;
      case 99:
        ppcVar18 = param_3 + 1;
        uVar9 = *(undefined *)((int)param_3 + 3);
        if ((1 < (int)pcVar16) && (!bVar3)) {
          PAD(param_1,(int)(pcVar16 + -1),0x20);
        }
        (*(code *)*param_1)(param_1,uVar9);
        if ((1 < (int)pcVar16) && (bVar3)) {
          PAD(param_1,(int)(pcVar16 + -1),0x20);
        }
        break;
      case 100:
      case 0x75:
        ppcVar18 = param_3 + 1;
        pcVar11 = *param_3;
        bVar19 = -1 < (int)pcVar11;
        if (!bVar19) {
          pcVar11 = (char *)-(int)pcVar11;
        }
        pcVar13 = local_26 + 1;
        local_26[1] = 0;
        do {
          pcVar13 = pcVar13 + -1;
          uVar14 = __modsi3((uint)pcVar11,10);
          *pcVar13 = (char)uVar14 + '0';
          pcVar11 = (char *)__divsi3((uint)pcVar11,10);
        } while (pcVar11 != (char *)0x0);
        if (bVar19) {
          if (bVar2) {
            cVar17 = '+';
          }
          else {
            cVar17 = '\0';
          }
        }
        else {
          cVar17 = '-';
        }
        sVar5 = strnlen(pcVar13,0x18);
        if (cVar17 != '\0') {
          sVar5 = sVar5 + 1;
        }
        if (((int)pcVar16 <= (int)sVar5) || (bVar3)) {
          if (cVar17 != '\0') {
            PAD(param_1,1,cVar17);
          }
        }
        else if (bVar4) {
          if (cVar17 != '\0') {
            PAD(param_1,1,cVar17);
          }
          PAD(param_1,(int)pcVar16 - sVar5,0x30);
        }
        else {
          PAD(param_1,(int)pcVar16 - sVar5,0x20);
          if (cVar17 != '\0') {
            PAD(param_1,1,cVar17);
          }
        }
        __writes(param_1,pcVar13);
        if (((int)sVar5 < (int)pcVar16) && (bVar3)) {
          PAD(param_1,(int)pcVar16 - sVar5,0x20);
        }
        break;
      case 0x73:
        ppcVar18 = param_3 + 1;
        pcVar11 = *param_3;
        if (pcVar11 == (char *)0x0) {
          pcVar11 = "(null)";
        }
LAB_0d402bb8:
        sVar5 = strnlen(pcVar11,0x400);
        if (((int)sVar5 < (int)pcVar16) && (!bVar3)) {
          PAD(param_1,(int)pcVar16 - sVar5,0x20);
        }
        __writes(param_1,pcVar11);
        if (((int)sVar5 < (int)pcVar16) && (bVar3)) {
          PAD(param_1,(int)pcVar16 - sVar5,0x20);
        }
      }
    }
    else {
      (*(code *)*param_1)(param_1,bVar1);
      ppcVar18 = param_3;
    }
    bVar1 = *pbVar15;
    pbVar15 = pbVar15 + 1;
    param_3 = ppcVar18;
  } while( true );
}



int vsnprintf(char *__s,size_t __maxlen,char *__format,__gnuc_va_list __arg)

{
  code *local_1c;
  undefined4 local_18;
  char *local_14;
  int local_10;
  size_t local_c;
  
  local_10 = 0;
  local_1c = __strWritec + 1;
  local_18 = 0xd402a29;
  local_14 = __s;
  local_c = __maxlen;
  __printf(&local_1c,(byte *)__format,(char **)__arg);
  return local_10;
}



int snprintf(char *__s,size_t __maxlen,char *__format,...)

{
  int iVar1;
  undefined4 in_r3;
  undefined4 uStack_4;
  
  uStack_4 = in_r3;
  iVar1 = vsnprintf(__s,__maxlen,__format,&uStack_4);
  return iVar1;
}



int memcmp(void *__s1,void *__s2,size_t __n)

{
  int iVar1;
  
  if (__n == 0) {
    iVar1 = 0;
  }
  else {
    iVar1 = __n - 1;
                    // WARNING: Load size is inaccurate
                    // WARNING: Load size is inaccurate
    if ((iVar1 != 0) && (*__s1 == *__s2)) {
      do {
        __s1 = (void *)((int)__s1 + 1);
        __s2 = (void *)((int)__s2 + 1);
        iVar1 = iVar1 + -1;
        if (iVar1 == 0) break;
                    // WARNING: Load size is inaccurate
                    // WARNING: Load size is inaccurate
      } while (*__s1 == *__s2);
    }
                    // WARNING: Load size is inaccurate
                    // WARNING: Load size is inaccurate
    iVar1 = (uint)*__s1 - (uint)*__s2;
  }
  return iVar1;
}



char * strncpy(char *__dest,char *__src,size_t __n)

{
  uint i;
  char *d;
  char s;
  
  if (__dest < (char *)0x1800000) {
    d = __dest;
    if (__n != 0) {
      do {
        __n = __n - 1;
        i = ((uint)d & 3) * -8 + 0x18;
        *(uint *)(d + -((uint)d & 3)) =
             *(uint *)(d + -((uint)d & 3)) & ~(0xff << (i & 0xff)) |
             (uint)(byte)*__src << (i & 0xff);
        d = d + 1;
        if (*__src == 0) break;
        __src = (char *)((byte *)__src + 1);
      } while (__n != 0);
    }
    for (; __n != 0; __n = __n - 1) {
      *(uint *)(d + -((uint)d & 3)) =
           *(uint *)(d + -((uint)d & 3)) & ~(0xff << (((uint)d & 3) * -8 + 0x18 & 0xff));
      d = d + 1;
    }
  }
  else {
    d = __dest;
    if (__n != 0) {
      do {
        __n = __n - 1;
        s = *__src;
        __src = __src + 1;
        *d = s;
        d = d + 1;
        if (s == '\0') break;
      } while (__n != 0);
    }
    for (; __n != 0; __n = __n - 1) {
      *d = '\0';
      d = d + 1;
    }
  }
  return __dest;
}



undefined4 __memsetc_unaligned(undefined *param_1,uint param_2,int param_3)

{
  uint uVar1;
  int iVar2;
  undefined4 in_lr;
  bool bVar3;
  
  if (param_1 < (undefined *)0x1800000) {
    iVar2 = param_3 + -1;
    if (param_3 != 0) {
      do {
        uVar1 = ((uint)param_1 & 3) * -8 + 0x18;
        *(uint *)(param_1 + -((uint)param_1 & 3)) =
             *(uint *)(param_1 + -((uint)param_1 & 3)) & ~(0xff << (uVar1 & 0xff)) |
             (param_2 & 0xff) << (uVar1 & 0xff);
        param_1 = param_1 + 1;
        bVar3 = iVar2 != 0;
        iVar2 = iVar2 + -1;
      } while (bVar3);
    }
  }
  else {
    iVar2 = param_3 + -1;
    if (param_3 != 0) {
      do {
        *param_1 = (char)(param_2 & 0xff);
        param_1 = param_1 + 1;
        bVar3 = iVar2 != 0;
        iVar2 = iVar2 + -1;
      } while (bVar3);
    }
  }
  return in_lr;
}



void * memset(void *__s,int __c,size_t __n)

{
  uint uVar1;
  uint uVar2;
  uint uVar3;
  uint uVar4;
  uint uVar5;
  uint *puVar6;
  uint *puVar7;
  
  if (__n != 0) {
    uVar1 = __c & 0xff;
    puVar6 = (uint *)__s;
    if (((uint)__s & 3) != 0) {
      uVar4 = 4 - ((uint)__s & 3);
      if (__n <= uVar4) {
        __memsetc_unaligned((undefined *)__s,uVar1,__n);
        return __s;
      }
      __memsetc_unaligned((undefined *)__s,uVar1,uVar4);
      __n = __n - uVar4;
      puVar6 = (uint *)(((uint)__s & 0xfffffffc) + 4);
    }
    if ((__n < 0x101) || (((uint)puVar6 & 0xf) == 0)) {
      uVar4 = __n & 0xfffffffc;
    }
    else {
      uVar4 = 0x10 - ((uint)puVar6 & 0xf);
      uVar5 = __n;
      if (__n < uVar4) {
        uVar5 = 0;
        uVar4 = __n;
      }
      uVar3 = uVar4 & 0xc;
      if (uVar3 != 0) {
        uVar2 = uVar1 | uVar1 << 8;
        puVar7 = puVar6;
        for (; 3 < uVar3; uVar3 = uVar3 - 4) {
          *puVar7 = uVar2 | uVar2 << 0x10;
          puVar7 = puVar7 + 1;
        }
        uVar4 = uVar4 & 3;
      }
      if (uVar4 != 0) {
        __memsetc_unaligned((undefined *)puVar6,uVar1,uVar4);
      }
      if (uVar5 == 0) {
        return __s;
      }
      __n = (uVar5 + ((uint)puVar6 & 0xf)) - 0x10;
      puVar6 = (uint *)(((uint)puVar6 & 0xfffffff0) + 0x10);
      uVar4 = __n & 0xfffffff0;
    }
    if (uVar4 != 0) {
      ____memsets_from_thumb(puVar6,uVar1,uVar4);
      puVar6 = (uint *)((int)puVar6 + uVar4);
      __n = __n - uVar4;
    }
    if (__n != 0) {
      __memsetc_unaligned((undefined *)puVar6,uVar1,__n);
    }
  }
  return __s;
}



char * strncat(char *__dest,char *__src,size_t __n)

{
  char cVar1;
  uint uVar2;
  char *pcVar3;
  int iVar4;
  char *pcVar5;
  bool bVar6;
  
  cVar1 = *__dest;
  pcVar3 = __dest;
  while (cVar1 != '\0') {
    pcVar3 = pcVar3 + 1;
    cVar1 = *pcVar3;
  }
  if (pcVar3 < (char *)0x1800000) {
    iVar4 = __n - 1;
    if (__n != 0) {
      do {
        uVar2 = ((uint)pcVar3 & 3) * -8 + 0x18;
        *(uint *)(pcVar3 + -((uint)pcVar3 & 3)) =
             *(uint *)(pcVar3 + -((uint)pcVar3 & 3)) & ~(0xff << (uVar2 & 0xff)) |
             (uint)(byte)*__src << (uVar2 & 0xff);
        pcVar3 = pcVar3 + 1;
        if (*__src == 0) {
          return __dest;
        }
        if (iVar4 == 0) {
          *pcVar3 = '\0';
        }
        __src = (char *)((byte *)__src + 1);
        bVar6 = iVar4 != 0;
        iVar4 = iVar4 + -1;
      } while (bVar6);
    }
  }
  else {
    iVar4 = __n - 1;
    if (__n != 0) {
      cVar1 = *__src;
      pcVar5 = __src + 1;
      *pcVar3 = cVar1;
      while (cVar1 != '\0') {
        pcVar3 = pcVar3 + 1;
        if (iVar4 == 0) {
          *pcVar3 = '\0';
        }
        bVar6 = iVar4 == 0;
        iVar4 = iVar4 + -1;
        if (bVar6) {
          return __dest;
        }
        cVar1 = *pcVar5;
        pcVar5 = pcVar5 + 1;
        *pcVar3 = cVar1;
      }
    }
  }
  return __dest;
}



size_t strnlen(char *__string,size_t __maxlen)

{
  char *pcVar1;
  int iVar2;
  bool bVar3;
  
  pcVar1 = __string;
  if ((*__string != '\0') && (iVar2 = __maxlen - 1, __maxlen != 0)) {
    do {
      pcVar1 = pcVar1 + 1;
      if (*pcVar1 == '\0') break;
      bVar3 = iVar2 != 0;
      iVar2 = iVar2 + -1;
    } while (bVar3);
  }
  return (int)pcVar1 - (int)__string;
}



int busDelay(int value)

{
  uint i;
  int in_lr;
  
  i = 0;
  if (value != 4) {
    do {
      i = i + 1;
    } while (i < value - 4U);
  }
  return in_lr;
}



undefined8 __sys_open(undefined4 param_1)

{
  software_interrupt(0xab);
  return CONCAT44(param_1,1);
}



undefined8 __sys_close(undefined4 param_1)

{
  software_interrupt(0xab);
  return CONCAT44(param_1,2);
}



undefined8 __sys_writec(undefined4 param_1)

{
  software_interrupt(0xab);
  return CONCAT44(param_1,3);
}



undefined8 __sys_write0(undefined4 param_1)

{
  software_interrupt(0xab);
  return CONCAT44(param_1,4);
}



undefined8 __sys_write(undefined4 param_1)

{
  software_interrupt(0xab);
  return CONCAT44(param_1,5);
}



undefined8 __sys_read(undefined4 param_1)

{
  software_interrupt(0xab);
  return CONCAT44(param_1,6);
}



undefined8 __sys_readc(undefined4 param_1)

{
  software_interrupt(0xab);
  return CONCAT44(param_1,7);
}



undefined8 __sys_iserror(undefined4 param_1)

{
  software_interrupt(0xab);
  return CONCAT44(param_1,8);
}



undefined8 __sys_istty(undefined4 param_1)

{
  software_interrupt(0xab);
  return CONCAT44(param_1,9);
}



undefined8 __sys_seek(undefined4 param_1)

{
  software_interrupt(0xab);
  return CONCAT44(param_1,10);
}



undefined8 __sys_flen(undefined4 param_1)

{
  software_interrupt(0xab);
  return CONCAT44(param_1,0xc);
}



undefined8 __sys_tmpnam(undefined4 param_1)

{
  software_interrupt(0xab);
  return CONCAT44(param_1,0xd);
}



undefined8 __sys_remove(undefined4 param_1)

{
  software_interrupt(0xab);
  return CONCAT44(param_1,0xe);
}



undefined8 __sys_rename(undefined4 param_1)

{
  software_interrupt(0xab);
  return CONCAT44(param_1,0xf);
}



undefined8 __sys_clocK(undefined4 param_1)

{
  software_interrupt(0xab);
  return CONCAT44(param_1,0x10);
}



undefined8 __sys_time(undefined4 param_1)

{
  software_interrupt(0xab);
  return CONCAT44(param_1,0x11);
}



undefined8 __sys_system(undefined4 param_1)

{
  software_interrupt(0xab);
  return CONCAT44(param_1,0x12);
}



undefined8 __sys_errno(undefined4 param_1)

{
  software_interrupt(0xab);
  return CONCAT44(param_1,0x13);
}



undefined8 __sys_get_cmdline(undefined4 param_1)

{
  software_interrupt(0xab);
  return CONCAT44(param_1,0x15);
}



undefined8 __sys_heapinfo(undefined4 param_1)

{
  software_interrupt(0xab);
  return CONCAT44(param_1,0x16);
}



undefined8 __sys_elapsed(undefined4 param_1)

{
  software_interrupt(0xab);
  return CONCAT44(param_1,0x30);
}



undefined8 __sys_tickfreq(undefined4 param_1)

{
  software_interrupt(0xab);
  return CONCAT44(param_1,0x31);
}



void __memsets(uint *param_1,uint param_2,uint param_3)

{
  uint uVar1;
  uint uVar2;
  
  uVar1 = param_2 | param_2 << 8;
  uVar1 = uVar1 | uVar1 << 0x10;
  for (uVar2 = param_3 & 0xfffffff0; uVar2 != 0; uVar2 = uVar2 - 0x10) {
    *param_1 = uVar1;
    param_1[1] = uVar1;
    param_1[2] = uVar1;
    param_1[3] = uVar1;
    param_1 = param_1 + 4;
  }
  for (uVar2 = param_3 & 0xf; uVar2 != 0; uVar2 = uVar2 - 4) {
    *param_1 = uVar1;
    param_1 = param_1 + 1;
  }
  return;
}



uint __udivsi3(uint param_1,uint param_2)

{
  uint uVar1;
  uint uVar2;
  
  if (param_2 == 0) {
    __div0();
    return 0;
  }
  uVar2 = 1;
  uVar1 = 0;
  if (param_2 <= param_1) {
    for (; (param_2 < 0x10000000 && (param_2 < param_1)); param_2 = param_2 << 4) {
      uVar2 = uVar2 << 4;
    }
    for (; (param_2 < 0x80000000 && (param_2 < param_1)); param_2 = param_2 << 1) {
      uVar2 = uVar2 << 1;
    }
    while( true ) {
      if (param_2 <= param_1) {
        param_1 = param_1 - param_2;
        uVar1 = uVar1 | uVar2;
      }
      if (param_2 >> 1 <= param_1) {
        param_1 = param_1 - (param_2 >> 1);
        uVar1 = uVar1 | uVar2 >> 1;
      }
      if (param_2 >> 2 <= param_1) {
        param_1 = param_1 - (param_2 >> 2);
        uVar1 = uVar1 | uVar2 >> 2;
      }
      if (param_2 >> 3 <= param_1) {
        param_1 = param_1 - (param_2 >> 3);
        uVar1 = uVar1 | uVar2 >> 3;
      }
      if ((param_1 == 0) || (uVar2 = uVar2 >> 4, uVar2 == 0)) break;
      param_2 = param_2 >> 4;
    }
  }
  return uVar1;
}



uint __divsi3(uint param_1,uint param_2)

{
  uint uVar1;
  uint uVar2;
  uint uVar3;
  
  if (param_2 == 0) {
    __div0();
    return 0;
  }
  uVar3 = param_1 ^ param_2;
  uVar2 = 1;
  uVar1 = 0;
  if ((int)param_2 < 0) {
    param_2 = -param_2;
  }
  if ((int)param_1 < 0) {
    param_1 = -param_1;
  }
  if (param_2 <= param_1) {
    for (; (param_2 < 0x10000000 && (param_2 < param_1)); param_2 = param_2 << 4) {
      uVar2 = uVar2 << 4;
    }
    for (; (param_2 < 0x80000000 && (param_2 < param_1)); param_2 = param_2 << 1) {
      uVar2 = uVar2 << 1;
    }
    while( true ) {
      if (param_2 <= param_1) {
        param_1 = param_1 - param_2;
        uVar1 = uVar1 | uVar2;
      }
      if (param_2 >> 1 <= param_1) {
        param_1 = param_1 - (param_2 >> 1);
        uVar1 = uVar1 | uVar2 >> 1;
      }
      if (param_2 >> 2 <= param_1) {
        param_1 = param_1 - (param_2 >> 2);
        uVar1 = uVar1 | uVar2 >> 2;
      }
      if (param_2 >> 3 <= param_1) {
        param_1 = param_1 - (param_2 >> 3);
        uVar1 = uVar1 | uVar2 >> 3;
      }
      if ((param_1 == 0) || (uVar2 = uVar2 >> 4, uVar2 == 0)) break;
      param_2 = param_2 >> 4;
    }
  }
  if ((int)uVar3 < 0) {
    uVar1 = -uVar1;
  }
  return uVar1;
}



uint __modsi3(uint param_1,uint param_2)

{
  uint uVar1;
  uint uVar2;
  uint uVar3;
  
  uVar3 = 1;
  if (param_2 == 0) {
    __div0();
    return 0;
  }
  if ((int)param_2 < 0) {
    param_2 = -param_2;
  }
  uVar1 = param_1;
  if ((int)param_1 < 0) {
    uVar1 = -param_1;
  }
  if (param_2 <= uVar1) {
    for (; (param_2 < 0x10000000 && (param_2 < uVar1)); param_2 = param_2 << 4) {
      uVar3 = uVar3 << 4;
    }
    for (; (param_2 < 0x80000000 && (param_2 < uVar1)); param_2 = param_2 << 1) {
      uVar3 = uVar3 << 1;
    }
    while( true ) {
      uVar2 = 0;
      if (param_2 <= uVar1) {
        uVar1 = uVar1 - param_2;
      }
      if (param_2 >> 1 <= uVar1) {
        uVar1 = uVar1 - (param_2 >> 1);
        uVar2 = uVar3 >> 1 | uVar3 << 0x1f;
      }
      if (param_2 >> 2 <= uVar1) {
        uVar1 = uVar1 - (param_2 >> 2);
        uVar2 = uVar2 | uVar3 >> 2 | uVar3 << 0x1e;
      }
      if (param_2 >> 3 <= uVar1) {
        uVar1 = uVar1 - (param_2 >> 3);
        uVar2 = uVar2 | uVar3 << 0x1d;
      }
      if ((uVar1 == 0) || (uVar3 >> 4 == 0)) break;
      param_2 = param_2 >> 4;
      uVar3 = uVar3 >> 4;
    }
    uVar2 = uVar2 & 0xe0000000;
    if ((uVar2 != 0) && ((uVar3 & 7) != 0)) {
      if ((uVar2 & uVar3 << 0x1d) != 0) {
        uVar1 = uVar1 + (param_2 >> 3);
      }
      if ((uVar2 & (uVar3 >> 2 | uVar3 << 0x1e)) != 0) {
        uVar1 = uVar1 + (param_2 >> 2);
      }
      if ((uVar2 & (uVar3 >> 1 | uVar3 << 0x1f)) != 0) {
        uVar1 = uVar1 + (param_2 >> 1);
      }
    }
  }
  if ((int)param_1 < 0) {
    uVar1 = -uVar1;
  }
  return uVar1;
}



void __div0(void)

{
  return;
}



undefined8 FUN_0d4034d6(byte *param_1,int param_2,int param_3,int param_4,int param_5,uint param_6)

{
  byte bVar1;
  byte bVar2;
  bool bVar3;
  undefined4 uVar4;
  byte *pbVar5;
  uint uVar6;
  undefined4 in_lr;
  undefined auStack_a38 [516];
  uint auStack_834 [129];
  uint auStack_630 [129];
  undefined auStack_42c [480];
  byte abStack_24c [36];
  char cStack_228;
  char cStack_227;
  byte abStack_226 [514];
  
  uVar6 = param_6 >> 2;
  OS2IP((int)auStack_42c,uVar6,param_2,param_6);
  OS2IP((int)auStack_630,uVar6,param_3,param_6);
  OS2IP((int)auStack_834,1,param_4,param_5);
  bigint_mod_exp((int)auStack_a38,auStack_630,auStack_834,1,(int)auStack_42c,uVar6);
  I2OSP((int)&cStack_228,param_6 & 0x1fffffff,(int)auStack_a38,uVar6);
  bVar3 = cStack_228 == '\0' && cStack_227 == '\x01';
  pbVar5 = abStack_226;
  if (pbVar5 < abStack_24c + param_6) {
    do {
      bVar1 = *pbVar5;
      pbVar5 = pbVar5 + 1;
      if (bVar1 != 0xff) {
        bVar3 = false;
      }
    } while (pbVar5 < abStack_24c + param_6);
  }
  uVar6 = 0;
  do {
    bVar1 = *pbVar5;
    pbVar5 = pbVar5 + 1;
    if (bVar1 != sha1_sig_0[uVar6]) {
      bVar3 = false;
    }
    uVar6 = uVar6 + 1;
  } while (uVar6 < 0x10);
  uVar6 = 0;
  do {
    bVar1 = *param_1;
    param_1 = param_1 + 1;
    bVar2 = *pbVar5;
    pbVar5 = pbVar5 + 1;
    if ((uint)bVar1 * 0x1000000 + (uint)bVar2 * -0x1000000 != 0) {
      bVar3 = false;
    }
    uVar6 = uVar6 + 1;
  } while (uVar6 < 0x14);
  if (bVar3) {
    uVar4 = 0;
  }
  else {
    uVar4 = 7;
  }
  return CONCAT44(in_lr,uVar4);
}



undefined8
CSL_VerifyRsaSig(byte *param_1,int param_2,int param_3,int param_4,int param_5,uint param_6)

{
  byte bVar1;
  byte bVar2;
  bool bVar3;
  undefined4 uVar4;
  byte *pbVar5;
  uint uVar6;
  undefined4 in_lr;
  undefined auStack_a38 [516];
  uint auStack_834 [129];
  uint auStack_630 [129];
  undefined auStack_42c [480];
  byte abStack_24c [36];
  char local_228;
  char local_227;
  byte local_226 [514];
  
  uVar6 = param_6 >> 2;
  OS2IP((int)auStack_42c,uVar6,param_2,param_6);
  OS2IP((int)auStack_630,uVar6,param_3,param_6);
  OS2IP((int)auStack_834,1,param_4,param_5);
  bigint_mod_exp((int)auStack_a38,auStack_630,auStack_834,1,(int)auStack_42c,uVar6);
  I2OSP((int)&local_228,param_6 & 0x1fffffff,(int)auStack_a38,uVar6);
  bVar3 = local_228 == '\0' && local_227 == '\x01';
  pbVar5 = local_226;
  if (pbVar5 < abStack_24c + param_6) {
    do {
      bVar1 = *pbVar5;
      pbVar5 = pbVar5 + 1;
      if (bVar1 != 0xff) {
        bVar3 = false;
      }
    } while (pbVar5 < abStack_24c + param_6);
  }
  uVar6 = 0;
  do {
    bVar1 = *pbVar5;
    pbVar5 = pbVar5 + 1;
    if (bVar1 != sha1_sig_0[uVar6]) {
      bVar3 = false;
    }
    uVar6 = uVar6 + 1;
  } while (uVar6 < 0x10);
  uVar6 = 0;
  do {
    bVar1 = *param_1;
    param_1 = param_1 + 1;
    bVar2 = *pbVar5;
    pbVar5 = pbVar5 + 1;
    if ((uint)bVar1 * 0x1000000 + (uint)bVar2 * -0x1000000 != 0) {
      bVar3 = false;
    }
    uVar6 = uVar6 + 1;
  } while (uVar6 < 0x14);
  if (bVar3) {
    uVar4 = 0;
  }
  else {
    uVar4 = 7;
  }
  return CONCAT44(in_lr,uVar4);
}



undefined8 CSL_VerifyRsaSig2048(byte *param_1,int param_2,int param_3,int param_4,int param_5)

{
  undefined4 in_lr;
  undefined8 uVar1;
  
  uVar1 = CSL_VerifyRsaSig(param_1,param_2,param_3,param_4,param_5,0x100);
  return CONCAT44(in_lr,(int)uVar1);
}



undefined8 CSL_VerifyRsaSig4096(byte *param_1,int param_2,int param_3,int param_4,int param_5)

{
  undefined4 in_lr;
  undefined8 uVar1;
  
  uVar1 = CSL_VerifyRsaSig(param_1,param_2,param_3,param_4,param_5,0x200);
  return CONCAT44(in_lr,(int)uVar1);
}



undefined4 CSL_ComputeRsaSig(int param_1,int param_2,int param_3,int param_4,uint param_5)

{
  uint uVar1;
  undefined4 in_lr;
  undefined auStack_834 [516];
  uint auStack_630 [129];
  uint auStack_42c [129];
  undefined auStack_228 [516];
  
  uVar1 = param_5 >> 2;
  OS2IP((int)auStack_228,uVar1,param_3,param_5);
  OS2IP((int)auStack_42c,uVar1,param_2,param_5);
  OS2IP((int)auStack_630,uVar1,param_4,param_5);
  bigint_mod_exp((int)auStack_834,auStack_42c,auStack_630,uVar1,(int)auStack_228,uVar1);
  I2OSP(param_1,param_5 & 0x1fffffff,(int)auStack_834,uVar1);
  return in_lr;
}



void bigint_digit_div(int *param_1,int *param_2,uint param_3)

{
  uint uVar1;
  uint uVar2;
  uint uVar3;
  uint uVar4;
  int iVar5;
  uint uVar6;
  uint uVar7;
  
  uVar3 = param_3 >> 0x10;
  uVar1 = param_3 & 0xffff;
  iVar5 = *param_2;
  uVar7 = param_2[1];
  if (uVar3 == 0xffff) {
    uVar2 = uVar7 >> 0x10;
  }
  else {
    uVar2 = __udivsi3(uVar7,uVar3 + 1);
    uVar2 = uVar2 & 0xffff;
  }
  uVar4 = uVar1 * uVar2;
  uVar6 = iVar5 + uVar4 * -0x10000;
  if (~(uVar4 * 0x10000) < uVar6) {
    uVar7 = uVar7 - 1;
  }
  uVar7 = (uVar7 - (uVar4 >> 0x10)) - uVar3 * uVar2;
  if ((uVar3 < uVar7) || ((uVar3 == uVar7 && (uVar1 * 0x10000 <= uVar6)))) {
    do {
      uVar6 = uVar6 + uVar1 * -0x10000;
      if (~(uVar1 * 0x10000) < uVar6) {
        uVar7 = uVar7 - 1;
      }
      uVar7 = uVar7 - uVar3;
      uVar2 = uVar2 + 1 & 0xffff;
    } while ((uVar3 < uVar7) || ((uVar3 == uVar7 && (uVar1 * 0x10000 <= uVar6))));
  }
  uVar4 = uVar7;
  if (uVar3 != 0xffff) {
    uVar4 = __udivsi3((uVar6 >> 0x10) + uVar7 * 0x10000,uVar3 + 1);
  }
  uVar4 = uVar4 & 0xffff;
  uVar3 = uVar3 * uVar4;
  uVar6 = uVar6 - uVar1 * uVar4;
  if (~(uVar1 * uVar4) < uVar6) {
    uVar7 = uVar7 - 1;
  }
  uVar6 = uVar6 + uVar3 * -0x10000;
  if (~(uVar3 * 0x10000) < uVar6) {
    uVar7 = uVar7 - 1;
  }
  iVar5 = uVar7 - (uVar3 >> 0x10);
  if ((iVar5 != 0) || (param_3 <= uVar6)) {
    do {
      uVar6 = uVar6 - param_3;
      if (~param_3 < uVar6) {
        iVar5 = iVar5 + -1;
      }
      uVar4 = uVar4 + 1 & 0xffff;
    } while ((iVar5 != 0) || (param_3 <= uVar6));
  }
  *param_1 = uVar4 + uVar2 * 0x10000;
  return;
}



void bigint_copy(int *a,int *b,int digits)

{
  int i;
  
  i = 0;
  if (digits < 1) {
    return;
  }
  do {
    a[i] = b[i];
    i = i + 1;
  } while (i < digits);
  return;
}



void bigint_zero(int a,int digits)

{
  int i;
  
  i = 0;
  if (0 < digits) {
    do {
      *(undefined4 *)(a + i * 4) = 0;
      i = i + 1;
    } while (i < digits);
    return;
  }
  return;
}



void bigint_add(int a,int b,int c,int d)

{
  uint carry;
  uint i;
  uint ai;
  int j;
  int k;
  
  carry = 0;
  k = 0;
  if (0 < d) {
    do {
      i = *(uint *)(b + k * 4);
      ai = carry + i;
      if (CARRY4(carry,i)) {
        j = *(int *)(c + k * 4);
      }
      else {
        carry = *(uint *)(c + k * 4);
        j = ai + carry;
        if (CARRY4(ai,carry)) {
          carry = 1;
        }
        else {
          carry = 0;
        }
      }
      *(int *)(a + k * 4) = j;
      k = k + 1;
    } while (k < d);
  }
  return;
}



void bigint_sub(int param_1,int param_2,int param_3,int param_4)

{
  int iVar1;
  uint uVar2;
  uint uVar3;
  int iVar4;
  
  iVar1 = 0;
  iVar4 = 0;
  if (0 < param_4) {
    do {
      uVar3 = *(int *)(param_2 + iVar4 * 4) - iVar1;
      if (0xffffffffU - iVar1 < uVar3) {
        uVar3 = ~*(uint *)(param_3 + iVar4 * 4);
      }
      else {
        uVar2 = *(uint *)(param_3 + iVar4 * 4);
        uVar3 = uVar3 - uVar2;
        if (~uVar2 < uVar3) {
          iVar1 = 1;
        }
        else {
          iVar1 = 0;
        }
      }
      *(uint *)(param_1 + iVar4 * 4) = uVar3;
      iVar4 = iVar4 + 1;
    } while (iVar4 < param_4);
  }
  return;
}



int bigint_digits(int param_1,int param_2)

{
  int iVar1;
  
  for (iVar1 = param_2 + -1; (-1 < iVar1 && (*(int *)(param_1 + iVar1 * 4) == 0));
      iVar1 = iVar1 + -1) {
  }
  return iVar1 + 1;
}



void bigint_mult(int param_1,int param_2,uint *param_3,int param_4)

{
  int iVar1;
  int digits;
  uint uVar2;
  int iVar3;
  
  bigint_zero((int)integer_math_res,param_4 << 1);
  iVar1 = bigint_digits(param_2,param_4);
  digits = bigint_digits((int)param_3,param_4);
  iVar3 = 0;
  if (0 < iVar1) {
    do {
      uVar2 = bigint_add_digit_mult
                        ((uint *)(integer_math_res + iVar3 * 4),
                         (uint *)(integer_math_res + iVar3 * 4),*(uint *)(param_2 + iVar3 * 4),
                         param_3,digits);
      *(uint *)(integer_math_res + (iVar3 + digits) * 4) =
           uVar2 + *(int *)(integer_math_res + (iVar3 + digits) * 4);
      iVar3 = iVar3 + 1;
    } while (iVar3 < iVar1);
  }
  bigint_copy((int *)param_1,(int *)integer_math_res,param_4 << 1);
  return;
}



uint bigint_left_shift(int param_1,int param_2,uint param_3,int param_4)

{
  uint uVar1;
  uint uVar2;
  int iVar3;
  
  if ((int)param_3 < 0x20) {
    uVar1 = 0;
    iVar3 = 0;
    if (0 < param_4) {
      do {
        uVar2 = *(uint *)(param_2 + iVar3 * 4);
        *(uint *)(param_1 + iVar3 * 4) = uVar1 | uVar2 << (param_3 & 0xff);
        if (param_3 == 0) {
          uVar1 = 0;
        }
        else {
          uVar1 = uVar2 >> (0x20 - param_3 & 0xff);
        }
        iVar3 = iVar3 + 1;
      } while (iVar3 < param_4);
    }
  }
  else {
    uVar1 = 0;
  }
  return uVar1;
}



uint bigint_right_shift(int param_1,int param_2,uint param_3,int param_4)

{
  uint uVar1;
  int iVar2;
  uint uVar3;
  
  if ((int)param_3 < 0x20) {
    uVar1 = 0;
    for (iVar2 = param_4 + -1; -1 < iVar2; iVar2 = iVar2 + -1) {
      uVar3 = *(uint *)(param_2 + iVar2 * 4);
      *(uint *)(param_1 + iVar2 * 4) = uVar1 | uVar3 >> (param_3 & 0xff);
      if (param_3 == 0) {
        uVar1 = 0;
      }
      else {
        uVar1 = uVar3 << (0x20 - param_3 & 0xff);
      }
    }
  }
  else {
    uVar1 = 0;
  }
  return uVar1;
}



int bigint_cmp(int a,int b,int digits)

{
  int i;
  
  i = digits + -1;
  while( true ) {
    if (i < 0) {
      return 0;
    }
    if (*(uint *)(b + i * 4) < *(uint *)(a + i * 4)) break;
    if (*(uint *)(a + i * 4) < *(uint *)(b + i * 4)) {
      return -1;
    }
    i = i + -1;
  }
  return 1;
}



void bigint_div(int param_1,int param_2,int param_3,int param_4,int param_5,int param_6)

{
  int digits;
  int iVar1;
  uint uVar2;
  uint uVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  uint local_30;
  
  digits = bigint_digits(param_5,param_6);
  if (digits != 0) {
    iVar1 = bigint_digit_bits(*(uint *)(digits * 4 + param_5 + -4));
    uVar2 = 0x20 - iVar1;
    bigint_zero((int)integer_math_cc,digits);
    uVar3 = bigint_left_shift((int)integer_math_cc,param_3,uVar2,param_4);
    *(uint *)(integer_math_cc + param_4 * 4) = uVar3;
    bigint_left_shift((int)integer_math_dd,param_5,uVar2,digits);
    iVar6 = *(int *)(integer_math_cc + digits * 4 + 0x408);
    bigint_zero(param_1,param_4);
    for (iVar1 = param_4 - digits; -1 < iVar1; iVar1 = iVar1 + -1) {
      if (iVar6 == -1) {
        local_30 = *(uint *)(integer_math_cc + (iVar1 + digits) * 4);
      }
      else {
        bigint_digit_div((int *)&local_30,(int *)(integer_math_tt + (iVar1 + digits) * 4 + 0x200),
                         iVar6 + 1);
      }
      iVar5 = iVar1 + digits;
      uVar3 = bigint_sub_digit_mult
                        ((int *)(integer_math_cc + iVar1 * 4),(uint *)(integer_math_cc + iVar1 * 4),
                         local_30,(uint *)integer_math_dd,digits);
      *(uint *)(integer_math_cc + iVar5 * 4) = *(int *)(integer_math_cc + iVar5 * 4) - uVar3;
      while ((*(int *)(integer_math_cc + iVar5 * 4) != 0 ||
             (iVar4 = bigint_cmp((int)(integer_math_cc + iVar1 * 4),(int)integer_math_dd,digits),
             -1 < iVar4))) {
        local_30 = local_30 + 1;
        iVar4 = bigint_sub((int)(integer_math_cc + iVar1 * 4),(int)(integer_math_cc + iVar1 * 4),
                           (int)integer_math_dd,digits);
        *(int *)(integer_math_cc + iVar5 * 4) = *(int *)(integer_math_cc + iVar5 * 4) - iVar4;
      }
      *(uint *)(param_1 + iVar1 * 4) = local_30;
    }
    bigint_zero(param_2,param_6);
    bigint_right_shift(param_2,(int)integer_math_cc,uVar2,digits);
  }
  return;
}



void bigint_mod(int param_1,int param_2,int param_3,int param_4,int param_5)

{
  bigint_div((int)integer_math_mod_t,param_1,param_2,param_3,param_4,param_5);
  return;
}



void bigint_mod_mult(int param_1,int param_2,uint *param_3,int param_4,int param_5)

{
  bigint_mult((int)integer_math_mod_mult_t,param_2,param_3,param_5);
  bigint_mod(param_1,(int)integer_math_mod_mult_t,param_5 << 1,param_4,param_5);
  return;
}



void bigint_mod_exp(int param_1,uint *param_2,uint *param_3,int param_4,int param_5,int param_6)

{
  int iVar1;
  uint uVar2;
  uint uVar3;
  int iVar4;
  uint uVar5;
  int local_3c [4];
  
  local_3c[0] = 1;
  local_3c[1] = 1;
  local_3c[2] = 0;
  local_3c[3] = 0;
  uVar2 = *param_3;
  iVar4 = 0;
  do {
    local_3c[uVar2 & 3] = local_3c[uVar2 & 3] + 1;
    uVar2 = uVar2 >> 2;
    iVar4 = iVar4 + 1;
  } while (iVar4 < 0x10);
  if (local_3c[3] != 0) {
    local_3c[2] = 1;
  }
  bigint_copy((int *)integer_math_b_power,(int *)param_2,param_6);
  if (local_3c[2] != 0) {
    bigint_mod_mult(0xd405a68,(int)integer_math_b_power,param_2,param_5,param_6);
  }
  if (local_3c[3] != 0) {
    bigint_mod_mult(0xd405c6c,0xd405a68,param_2,param_5,param_6);
  }
  bigint_zero((int)integer_math_tt,param_6);
  integer_math_tt._0_4_ = 1;
  iVar1 = bigint_digits((int)param_3,param_4);
  iVar4 = iVar1;
  while (iVar4 = iVar4 + -1, -1 < iVar4) {
    uVar2 = param_3[iVar4];
    uVar5 = 0x20;
    if (iVar1 + -1 == iVar4) {
      for (; uVar2 >> 0x1e == 0; uVar2 = uVar2 << 2) {
        uVar5 = uVar5 - 2;
      }
    }
    uVar3 = 0;
    if (uVar5 != 0) {
      do {
        bigint_mod_mult((int)integer_math_tt,(int)integer_math_tt,(uint *)integer_math_tt,param_5,
                        param_6);
        bigint_mod_mult((int)integer_math_tt,(int)integer_math_tt,(uint *)integer_math_tt,param_5,
                        param_6);
        if (uVar2 >> 0x1e != 0) {
          bigint_mod_mult((int)integer_math_tt,(int)integer_math_tt,
                          (uint *)(integer_math_res + (uVar2 >> 0x1e) * 0x204 + 0x204),param_5,
                          param_6);
        }
        uVar3 = uVar3 + 2;
        uVar2 = uVar2 << 2;
      } while (uVar3 < uVar5);
    }
  }
  bigint_copy((int *)param_1,(int *)integer_math_tt,param_6);
  return;
}



undefined4 bigint_iszero(int param_1,int param_2)

{
  int iVar1;
  
  iVar1 = 0;
  if (0 < param_2) {
    do {
      if (*(int *)(param_1 + iVar1 * 4) != 0) {
        return 0;
      }
      iVar1 = iVar1 + 1;
    } while (iVar1 < param_2);
  }
  return 1;
}



void bigint_mod_inv(int param_1,int param_2,int param_3,int param_4)

{
  int iVar1;
  int iVar2;
  undefined auStack_14c [64];
  int aiStack_10c [8];
  uint auStack_ec [8];
  int aiStack_cc [8];
  int local_ac [8];
  int aiStack_8c [8];
  int aiStack_6c [8];
  undefined auStack_4c [32];
  
  bigint_zero((int)local_ac,param_4);
  local_ac[0] = 1;
  bigint_zero((int)auStack_ec,param_4);
  bigint_copy(aiStack_cc,(int *)param_2,param_4);
  bigint_copy(aiStack_10c,(int *)param_3,param_4);
  iVar2 = 1;
  while (iVar1 = bigint_iszero((int)aiStack_10c,param_4), iVar1 == 0) {
    bigint_div((int)auStack_4c,(int)aiStack_8c,(int)aiStack_cc,param_4,(int)aiStack_10c,param_4);
    bigint_mult((int)auStack_14c,(int)auStack_4c,auStack_ec,param_4);
    bigint_add((int)aiStack_6c,(int)local_ac,(int)auStack_14c,param_4);
    bigint_copy(local_ac,(int *)auStack_ec,param_4);
    bigint_copy((int *)auStack_ec,aiStack_6c,param_4);
    bigint_copy(aiStack_cc,aiStack_10c,param_4);
    bigint_copy(aiStack_10c,aiStack_8c,param_4);
    iVar2 = -iVar2;
  }
  if (iVar2 < 0) {
    bigint_sub(param_1,param_3,(int)local_ac,param_4);
  }
  else {
    bigint_copy((int *)param_1,local_ac,param_4);
  }
  return;
}



undefined4 I2OSP(int param_1,int param_2,int param_3,uint param_4)

{
  uint uVar1;
  int iVar2;
  uint uVar3;
  uint uVar4;
  undefined4 in_lr;
  
  uVar4 = 0;
  iVar2 = param_2 + -1;
  if (param_4 != 0) {
    do {
      if (iVar2 < 0) {
        return in_lr;
      }
      uVar1 = *(uint *)(uVar4 * 4 + param_3);
      uVar3 = 0;
      if (-1 < iVar2) {
        do {
          *(char *)(param_1 + iVar2) = (char)(uVar1 >> (uVar3 & 0xff));
          uVar3 = uVar3 + 8;
          iVar2 = iVar2 + -1;
          if (iVar2 < 0) break;
        } while (uVar3 < 0x20);
      }
      uVar4 = uVar4 + 1;
    } while (uVar4 < param_4);
  }
  for (; -1 < iVar2; iVar2 = iVar2 + -1) {
    *(undefined *)(param_1 + iVar2) = 0;
  }
  return in_lr;
}



undefined4 OS2IP(int param_1,int param_2,int param_3,int param_4)

{
  uint uVar1;
  uint uVar2;
  int iVar3;
  int iVar4;
  undefined4 in_lr;
  
  iVar4 = 0;
  iVar3 = param_4 + -1;
  if (0 < param_2) {
    do {
      if (iVar3 < 0) {
        for (; iVar4 < param_2; iVar4 = iVar4 + 1) {
          *(undefined4 *)(iVar4 * 4 + param_1) = 0;
        }
        return in_lr;
      }
      uVar1 = 0;
      uVar2 = 0;
      if (-1 < iVar3) {
        do {
          uVar1 = uVar1 | (uint)*(byte *)(param_3 + iVar3) << (uVar2 & 0xff);
          uVar2 = uVar2 + 8;
          iVar3 = iVar3 + -1;
          if (iVar3 < 0) break;
        } while ((int)uVar2 < 0x20);
      }
      *(uint *)(iVar4 * 4 + param_1) = uVar1;
      iVar4 = iVar4 + 1;
    } while (iVar4 < param_2);
  }
  return in_lr;
}



uint bigint_add_digit_mult(uint *a,uint *b,uint c,uint *d,int digits)

{
  uint carry;
  int i;
  bool j;
  longlong t;
  
  if (c == 0) {
    carry = 0;
  }
  else {
    carry = 0;
    do {
      t = (ulonglong)*d * (ulonglong)c + (ulonglong)carry + (ulonglong)*b;
      *a = (uint)t;
      carry = (uint)((ulonglong)t >> 0x20);
      i = digits + -1;
      j = 0 < digits;
      a = a + 1;
      b = b + 1;
      d = d + 1;
      digits = i;
    } while (i != 0 && j);
  }
  return carry;
}



uint bigint_sub_digit_mult(int *param_1,uint *param_2,uint param_3,uint *param_4,int param_5)

{
  bool bVar1;
  longlong lVar2;
  uint uVar3;
  uint uVar4;
  uint uVar5;
  int iVar6;
  
  if (param_3 == 0) {
    uVar3 = 0;
  }
  else {
    uVar3 = 0;
    do {
      uVar5 = *param_2;
      lVar2 = (ulonglong)*param_4 * (ulonglong)param_3 + (ulonglong)uVar3;
      uVar4 = (uint)lVar2;
      uVar3 = (uint)((ulonglong)lVar2 >> 0x20);
      *param_1 = uVar5 - uVar4;
      if (uVar5 < uVar4) {
        uVar3 = uVar3 + 1;
      }
      iVar6 = param_5 + -1;
      bVar1 = 0 < param_5;
      param_1 = param_1 + 1;
      param_2 = param_2 + 1;
      param_4 = param_4 + 1;
      param_5 = iVar6;
    } while (iVar6 != 0 && bVar1);
  }
  return uVar3;
}



int bigint_digit_bits(uint a)

{
  return 0x20 - LZCOUNT(a);
}



undefined8 __bootc_from_arm(void)

{
  undefined4 extraout_r0;
  undefined4 extraout_r1;
  
  bootc();
  return CONCAT44(extraout_r1,extraout_r0);
}



void * __memcpy_from_thumb(void *__dest,void *__src,size_t __n)

{
  undefined4 uVar1;
  uint uVar2;
  undefined4 uVar3;
  undefined4 uVar4;
  undefined4 uVar5;
  uint *puVar6;
  undefined4 uVar7;
  undefined4 uVar8;
  undefined4 uVar9;
  undefined4 uVar10;
  
  if (__n != 0) {
    if ((((uint)__dest | (uint)__src) & 3) == 0) {
      if (0xf < __n) {
        if (0x1f < __n) {
          do {
                    // WARNING: Load size is inaccurate
            uVar1 = *__src;
            uVar3 = *(undefined4 *)((int)__src + 4);
            uVar4 = *(undefined4 *)((int)__src + 8);
            uVar5 = *(undefined4 *)((int)__src + 0xc);
            uVar7 = *(undefined4 *)((int)__src + 0x10);
            uVar8 = *(undefined4 *)((int)__src + 0x14);
            uVar9 = *(undefined4 *)((int)__src + 0x18);
            uVar10 = *(undefined4 *)((int)__src + 0x1c);
            __src = (void *)((int)__src + 0x20);
            *(undefined4 *)__dest = uVar1;
            *(undefined4 *)((int)__dest + 4) = uVar3;
            *(undefined4 *)((int)__dest + 8) = uVar4;
            *(undefined4 *)((int)__dest + 0xc) = uVar5;
            *(undefined4 *)((int)__dest + 0x10) = uVar7;
            *(undefined4 *)((int)__dest + 0x14) = uVar8;
            *(undefined4 *)((int)__dest + 0x18) = uVar9;
            *(undefined4 *)((int)__dest + 0x1c) = uVar10;
            __dest = (void *)((int)__dest + 0x20);
            __n = __n - 0x20;
          } while (0x1f < __n);
        }
        for (; 0xf < __n; __n = __n - 0x10) {
                    // WARNING: Load size is inaccurate
          uVar1 = *__src;
          uVar3 = *(undefined4 *)((int)__src + 4);
          uVar4 = *(undefined4 *)((int)__src + 8);
          uVar5 = *(undefined4 *)((int)__src + 0xc);
          __src = (void *)((int)__src + 0x10);
          *(undefined4 *)__dest = uVar1;
          *(undefined4 *)((int)__dest + 4) = uVar3;
          *(undefined4 *)((int)__dest + 8) = uVar4;
          *(undefined4 *)((int)__dest + 0xc) = uVar5;
          __dest = (void *)((int)__dest + 0x10);
        }
      }
      for (; 3 < __n; __n = __n - 4) {
                    // WARNING: Load size is inaccurate
        *(undefined4 *)__dest = *__src;
        __dest = (undefined4 *)((int)__dest + 4);
        __src = (undefined4 *)((int)__src + 4);
      }
    }
    if (__dest < (undefined4 *)0x1800000) {
      for (; __n != 0; __n = __n - 1) {
        puVar6 = (uint *)((int)__dest - ((uint)__dest & 3));
        uVar2 = ((uint)__dest & 3) * -8 + 0x18;
                    // WARNING: Load size is inaccurate
        *puVar6 = *puVar6 & ~(0xff << (uVar2 & 0xff)) | (uint)*__src << (uVar2 & 0xff);
        __dest = (void *)((int)__dest + 1);
        __src = (undefined4 *)((int)__src + 1);
      }
    }
    else {
      for (; __n != 0; __n = __n - 1) {
                    // WARNING: Load size is inaccurate
        *(byte *)__dest = *__src;
        __dest = (undefined4 *)((int)__dest + 1);
        __src = (undefined4 *)((int)__src + 1);
      }
    }
    return __dest;
  }
  return __dest;
}



void * __memcpy_change_to_arm(void *__dest,void *__src,size_t __n)

{
  undefined4 uVar1;
  uint uVar2;
  undefined4 uVar3;
  undefined4 uVar4;
  undefined4 uVar5;
  uint *puVar6;
  undefined4 uVar7;
  undefined4 uVar8;
  undefined4 uVar9;
  undefined4 uVar10;
  
  if (__n != 0) {
    if ((((uint)__dest | (uint)__src) & 3) == 0) {
      if (0xf < __n) {
        if (0x1f < __n) {
          do {
                    // WARNING: Load size is inaccurate
            uVar1 = *__src;
            uVar3 = *(undefined4 *)((int)__src + 4);
            uVar4 = *(undefined4 *)((int)__src + 8);
            uVar5 = *(undefined4 *)((int)__src + 0xc);
            uVar7 = *(undefined4 *)((int)__src + 0x10);
            uVar8 = *(undefined4 *)((int)__src + 0x14);
            uVar9 = *(undefined4 *)((int)__src + 0x18);
            uVar10 = *(undefined4 *)((int)__src + 0x1c);
            __src = (void *)((int)__src + 0x20);
            *(undefined4 *)__dest = uVar1;
            *(undefined4 *)((int)__dest + 4) = uVar3;
            *(undefined4 *)((int)__dest + 8) = uVar4;
            *(undefined4 *)((int)__dest + 0xc) = uVar5;
            *(undefined4 *)((int)__dest + 0x10) = uVar7;
            *(undefined4 *)((int)__dest + 0x14) = uVar8;
            *(undefined4 *)((int)__dest + 0x18) = uVar9;
            *(undefined4 *)((int)__dest + 0x1c) = uVar10;
            __dest = (void *)((int)__dest + 0x20);
            __n = __n - 0x20;
          } while (0x1f < __n);
        }
        for (; 0xf < __n; __n = __n - 0x10) {
                    // WARNING: Load size is inaccurate
          uVar1 = *__src;
          uVar3 = *(undefined4 *)((int)__src + 4);
          uVar4 = *(undefined4 *)((int)__src + 8);
          uVar5 = *(undefined4 *)((int)__src + 0xc);
          __src = (void *)((int)__src + 0x10);
          *(undefined4 *)__dest = uVar1;
          *(undefined4 *)((int)__dest + 4) = uVar3;
          *(undefined4 *)((int)__dest + 8) = uVar4;
          *(undefined4 *)((int)__dest + 0xc) = uVar5;
          __dest = (void *)((int)__dest + 0x10);
        }
      }
      for (; 3 < __n; __n = __n - 4) {
                    // WARNING: Load size is inaccurate
        *(undefined4 *)__dest = *__src;
        __dest = (undefined4 *)((int)__dest + 4);
        __src = (undefined4 *)((int)__src + 4);
      }
    }
    if (__dest < (undefined4 *)0x1800000) {
      for (; __n != 0; __n = __n - 1) {
        puVar6 = (uint *)((int)__dest - ((uint)__dest & 3));
        uVar2 = ((uint)__dest & 3) * -8 + 0x18;
                    // WARNING: Load size is inaccurate
        *puVar6 = *puVar6 & ~(0xff << (uVar2 & 0xff)) | (uint)*__src << (uVar2 & 0xff);
        __dest = (void *)((int)__dest + 1);
        __src = (undefined4 *)((int)__src + 1);
      }
    }
    else {
      for (; __n != 0; __n = __n - 1) {
                    // WARNING: Load size is inaccurate
        *(byte *)__dest = *__src;
        __dest = (undefined4 *)((int)__dest + 1);
        __src = (undefined4 *)((int)__src + 1);
      }
    }
    return __dest;
  }
  return __dest;
}



void ____memsets_from_thumb(uint *param_1,uint param_2,uint param_3)

{
  __memsets(param_1,param_2,param_3);
  return;
}



void ____memsets_change_to_arm(uint *param_1,uint param_2,uint param_3)

{
  __memsets(param_1,param_2,param_3);
  return;
}


