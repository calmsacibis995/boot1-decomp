typedef unsigned char   undefined;

typedef unsigned char    byte;
typedef unsigned int    dword;
typedef unsigned int    uint;
typedef unsigned long    ulong;
typedef unsigned long long    ulonglong;
typedef unsigned char    undefined1;
typedef unsigned short    undefined2;
typedef unsigned int    undefined4;
typedef unsigned long long    undefined8;
typedef unsigned short    ushort;
typedef unsigned short    word;
#define unkbyte9   unsigned long long
#define unkbyte10   unsigned long long
#define unkbyte11   unsigned long long
#define unkbyte12   unsigned long long
#define unkbyte13   unsigned long long
#define unkbyte14   unsigned long long
#define unkbyte15   unsigned long long
#define unkbyte16   unsigned long long

#define unkuint9   unsigned long long
#define unkuint10   unsigned long long
#define unkuint11   unsigned long long
#define unkuint12   unsigned long long
#define unkuint13   unsigned long long
#define unkuint14   unsigned long long
#define unkuint15   unsigned long long
#define unkuint16   unsigned long long

#define unkint9   long long
#define unkint10   long long
#define unkint11   long long
#define unkint12   long long
#define unkint13   long long
#define unkint14   long long
#define unkint15   long long
#define unkint16   long long

#define unkfloat1   float
#define unkfloat2   float
#define unkfloat3   float
#define unkfloat5   double
#define unkfloat6   double
#define unkfloat7   double
#define unkfloat9   long double
#define unkfloat11   long double
#define unkfloat12   long double
#define unkfloat13   long double
#define unkfloat14   long double
#define unkfloat15   long double
#define unkfloat16   long double

#define BADSPACEBASE   void
#define code   void

typedef ulong size_t;

typedef void * __gnuc_va_list;

typedef struct Elf32_Shdr Elf32_Shdr, *PElf32_Shdr;

typedef enum Elf_SectionHeaderType_ARM {
    SHT_NULL=0,
    SHT_PROGBITS=1,
    SHT_SYMTAB=2,
    SHT_STRTAB=3,
    SHT_RELA=4,
    SHT_HASH=5,
    SHT_DYNAMIC=6,
    SHT_NOTE=7,
    SHT_NOBITS=8,
    SHT_REL=9,
    SHT_SHLIB=10,
    SHT_DYNSYM=11,
    SHT_INIT_ARRAY=14,
    SHT_FINI_ARRAY=15,
    SHT_PREINIT_ARRAY=16,
    SHT_GROUP=17,
    SHT_SYMTAB_SHNDX=18,
    SHT_ANDROID_REL=1610612737,
    SHT_ANDROID_RELA=1610612738,
    SHT_GNU_ATTRIBUTES=1879048181,
    SHT_GNU_HASH=1879048182,
    SHT_GNU_LIBLIST=1879048183,
    SHT_CHECKSUM=1879048184,
    SHT_SUNW_move=1879048186,
    SHT_SUNW_COMDAT=1879048187,
    SHT_SUNW_syminfo=1879048188,
    SHT_GNU_verdef=1879048189,
    SHT_GNU_verneed=1879048190,
    SHT_GNU_versym=1879048191,
    SHT_ARM_EXIDX=1879048193,
    SHT_ARM_PREEMPTMAP=1879048194,
    SHT_ARM_ATTRIBUTES=1879048195,
    SHT_ARM_DEBUGOVERLAY=1879048196,
    SHT_ARM_OVERLAYSECTION=1879048197
} Elf_SectionHeaderType_ARM;

struct Elf32_Shdr {
    dword sh_name;
    enum Elf_SectionHeaderType_ARM sh_type;
    dword sh_flags;
    dword sh_addr;
    dword sh_offset;
    dword sh_size;
    dword sh_link;
    dword sh_info;
    dword sh_addralign;
    dword sh_entsize;
};

typedef struct Elf32_Sym Elf32_Sym, *PElf32_Sym;

struct Elf32_Sym {
    dword st_name;
    dword st_value;
    dword st_size;
    byte st_info;
    byte st_other;
    word st_shndx;
};

typedef struct Elf32_Phdr Elf32_Phdr, *PElf32_Phdr;

typedef enum Elf_ProgramHeaderType_ARM {
    PT_NULL=0,
    PT_LOAD=1,
    PT_DYNAMIC=2,
    PT_INTERP=3,
    PT_NOTE=4,
    PT_SHLIB=5,
    PT_PHDR=6,
    PT_TLS=7,
    PT_GNU_EH_FRAME=1685382480,
    PT_GNU_STACK=1685382481,
    PT_GNU_RELRO=1685382482,
    PT_ARM_EXIDX=1879048192
} Elf_ProgramHeaderType_ARM;

struct Elf32_Phdr {
    enum Elf_ProgramHeaderType_ARM p_type;
    dword p_offset;
    dword p_vaddr;
    dword p_paddr;
    dword p_filesz;
    dword p_memsz;
    dword p_flags;
    dword p_align;
};

typedef struct Elf32_Ehdr Elf32_Ehdr, *PElf32_Ehdr;

struct Elf32_Ehdr {
    byte e_ident_magic_num;
    char e_ident_magic_str[3];
    byte e_ident_class;
    byte e_ident_data;
    byte e_ident_version;
    byte e_ident_osabi;
    byte e_ident_abiversion;
    byte e_ident_pad[7];
    word e_type;
    word e_machine;
    dword e_version;
    dword e_entry;
    dword e_phoff;
    dword e_shoff;
    dword e_flags;
    word e_ehsize;
    word e_phentsize;
    word e_phnum;
    word e_shentsize;
    word e_shnum;
    word e_shstrndx;
};




void entry(void);
void word_set(undefined4 *param_1,undefined4 *param_2,undefined4 param_3,int param_4);
void __outputLed(int param_1);
void bootFailure(uint cause);
int setupAes(undefined4 *param_1,undefined4 *param_2,uint base);
undefined8 runHashAsync(uint *param_1,uint param_2,uint param_3,uint param_4,int param_5);
int checkEcc(int buf,int spare,uint pagesize);
undefined4 readPageAsync(undefined4 param_1,undefined4 param_2,undefined4 param_3);
undefined8 getExpSize(int param_1,int param_2);
int verifySig(void *base,void *p,void *issuer,int pubkey,int exp,uint type,uint size);
undefined8 bootc(void);
undefined4 EE_GPIOSetup(void);
void eeClkHigh(void);
void eeClkLow(void);
void eeCSHigh(void);
void eeDataOut(uint param_1);
undefined4 eeBitTimeIdle(int param_1);
undefined8 eeWrNbits(int param_1,uint param_2);
undefined8 eeRdNbits(int param_1,uint *param_2);
undefined8 eeChkStatus(void);
undefined8 EE_RdCmd(uint param_1,uint *param_2);
undefined8 EE_WrCmd(uint param_1,uint param_2);
undefined8 EE_EraseCmd(uint param_1);
undefined8 EE_WralCmd(uint param_1);
undefined8 EE_EralCmd(void);
undefined8 EE_WenCmd(void);
undefined8 EE_WdsCmd(void);
undefined8 eeCommitMainData(ushort *param_1,uint param_2);
ulonglong eeComputeMainDataSum(ushort *param_1);
ulonglong eeComputeSfsDataSum(ushort *param_1);
undefined8 eeRetrieveMainData(void *param_1,uint *param_2);
undefined8 eeRetrieveSfsData(void *param_1,uint *param_2);
undefined8 EE_GetOsVersion(void);
undefined8 EE_GetCaCrlVersion(void);
undefined8 EE_GetSignerCrlVersion(void);
undefined8 EE_GetSfsVersion(void);
undefined8 EE_IncOsVersion(void);
undefined8 EE_IncCaCrlVersion(void);
undefined8 EE_IncSignerCrlVersion(void);
undefined8 EE_IncSfsVersion(void);
undefined8 EE_GetRngState(void);
undefined8 EE_IncRngState(void);
void getChipId(uint *param_1,uint *param_2);
undefined8 _busRdAHMReg(uint param_1);
undefined8 rdbf(int param_1);
uint busRdMEMReg(uint a);
int ahbMemFlush(int groupname);
undefined8 busRdAHMReg(uint param_1);
uint ahbMemRBInvalidate(int buffer);
void gpioEnableIOPInt(uint param_1);
void gpioSetIOPIntPol(uint param_1);
void gpioSetOwner(undefined4 param_1);
void gpioSet(uint param_1,uint param_2);
void gpioSetOutputEnable(undefined4 param_1);
void gpioPPCSet(uint param_1,uint param_2);
void gpioPPCSetOutputEnable(undefined4 param_1);
void gpioEnable(undefined4 param_1);
int initGPIO(void);
int outputLed(int value);
undefined4 gpioCompatSwitch(void);
void __init_DDR_Napa(int param_1,uint param_2,undefined4 param_3,char param_4,char param_5);
void FUN_0d401b3e(undefined param_1,undefined param_2,undefined param_3,undefined param_4,undefined param_5,undefined param_6,undefined param_7,undefined param_8,uint param_9);
void FUN_0d401c1e(int param_1,undefined param_2,undefined param_3,undefined param_4,int param_5,int param_6,int param_7,undefined param_8,undefined param_9,undefined param_10,uint param_11);
void FUN_0d40246c(void);
void FUN_0d4024d8(int param_1);
int init_DDR_Napa(uint compat,char emu);
undefined4 set_mem_compat_mode(void);
int initARAMEmulation(void);
void writeDirectMemReg(uint index,uint data);
uint readDirectMemReg(uint index);
int writeDDRMemReg(uint index,uint data);
undefined8 readDDRMemReg(ushort param_1);
int writeSeqMemReg(uint index,uint data);
undefined8 readSeqMemReg(ushort param_1);
undefined4 readACRReg(int param_1);
void writeACRReg(uint index,uint data);
undefined4 FUN_0d40268a(void);
int udelay(uint usec);
undefined4 DIStraps(void);
undefined8 getRsvdSRAMStart(void);
uint isSSConfigValid(uint param_1);
undefined4 setSS(uint param_1);
uint convertMicrosecondsToTicks(uint us);
undefined8 getSysClk(void);
void * memcpy(void *__dest,void *__src,size_t __n);
undefined8 __ioFlush(int param_1,undefined4 param_2);
undefined4 __ioWritec(int param_1,undefined param_2);
int vprintf(char *__format,__gnuc_va_list __arg);
int printf(char *__format,...);
undefined4 __strFlush(int param_1);
undefined4 __strWritec(int param_1,undefined param_2);
undefined4 __writes(undefined **param_1,char *param_2);
int PAD(undefined **param_1,int param_2,undefined param_3);
undefined4 __printf(undefined **param_1,byte *param_2,char **param_3);
int vsnprintf(char *__s,size_t __maxlen,char *__format,__gnuc_va_list __arg);
int snprintf(char *__s,size_t __maxlen,char *__format,...);
int memcmp(void *__s1,void *__s2,size_t __n);
char * strncpy(char *__dest,char *__src,size_t __n);
undefined4 __memsetc_unaligned(undefined *param_1,uint param_2,int param_3);
void * memset(void *__s,int __c,size_t __n);
char * strncat(char *__dest,char *__src,size_t __n);
size_t strnlen(char *__string,size_t __maxlen);
int busDelay(int value);
undefined8 __sys_open(undefined4 param_1);
undefined8 __sys_close(undefined4 param_1);
undefined8 __sys_writec(undefined4 param_1);
undefined8 __sys_write0(undefined4 param_1);
undefined8 __sys_write(undefined4 param_1);
undefined8 __sys_read(undefined4 param_1);
undefined8 __sys_readc(undefined4 param_1);
undefined8 __sys_iserror(undefined4 param_1);
undefined8 __sys_istty(undefined4 param_1);
undefined8 __sys_seek(undefined4 param_1);
undefined8 __sys_flen(undefined4 param_1);
undefined8 __sys_tmpnam(undefined4 param_1);
undefined8 __sys_remove(undefined4 param_1);
undefined8 __sys_rename(undefined4 param_1);
undefined8 __sys_clocK(undefined4 param_1);
undefined8 __sys_time(undefined4 param_1);
undefined8 __sys_system(undefined4 param_1);
undefined8 __sys_errno(undefined4 param_1);
undefined8 __sys_get_cmdline(undefined4 param_1);
undefined8 __sys_heapinfo(undefined4 param_1);
undefined8 __sys_elapsed(undefined4 param_1);
undefined8 __sys_tickfreq(undefined4 param_1);
void __memsets(uint *param_1,uint param_2,uint param_3);
uint __udivsi3(uint param_1,uint param_2);
uint __divsi3(uint param_1,uint param_2);
uint __modsi3(uint param_1,uint param_2);
void __div0(void);
undefined8 FUN_0d4034d6(byte *param_1,int param_2,int param_3,int param_4,int param_5,uint param_6);
undefined8 CSL_VerifyRsaSig(byte *param_1,int param_2,int param_3,int param_4,int param_5,uint param_6);
undefined8 CSL_VerifyRsaSig2048(byte *param_1,int param_2,int param_3,int param_4,int param_5);
undefined8 CSL_VerifyRsaSig4096(byte *param_1,int param_2,int param_3,int param_4,int param_5);
undefined4 CSL_ComputeRsaSig(int param_1,int param_2,int param_3,int param_4,uint param_5);
void bigint_digit_div(int *param_1,int *param_2,uint param_3);
void bigint_copy(int *a,int *b,int digits);
void bigint_zero(int a,int digits);
void bigint_add(int a,int b,int c,int d);
void bigint_sub(int param_1,int param_2,int param_3,int param_4);
int bigint_digits(int param_1,int param_2);
void bigint_mult(int param_1,int param_2,uint *param_3,int param_4);
uint bigint_left_shift(int param_1,int param_2,uint param_3,int param_4);
uint bigint_right_shift(int param_1,int param_2,uint param_3,int param_4);
int bigint_cmp(int a,int b,int digits);
void bigint_div(int param_1,int param_2,int param_3,int param_4,int param_5,int param_6);
void bigint_mod(int param_1,int param_2,int param_3,int param_4,int param_5);
void bigint_mod_mult(int param_1,int param_2,uint *param_3,int param_4,int param_5);
void bigint_mod_exp(int param_1,uint *param_2,uint *param_3,int param_4,int param_5,int param_6);
undefined4 bigint_iszero(int param_1,int param_2);
void bigint_mod_inv(int param_1,int param_2,int param_3,int param_4);
undefined4 I2OSP(int param_1,int param_2,int param_3,uint param_4);
undefined4 OS2IP(int param_1,int param_2,int param_3,int param_4);
uint bigint_add_digit_mult(uint *a,uint *b,uint c,uint *d,int digits);
uint bigint_sub_digit_mult(int *param_1,uint *param_2,uint param_3,uint *param_4,int param_5);
int bigint_digit_bits(uint a);
undefined8 __bootc_from_arm(void);
void * __memcpy_from_thumb(void *__dest,void *__src,size_t __n);
void * __memcpy_change_to_arm(void *__dest,void *__src,size_t __n);
void ____memsets_from_thumb(uint *param_1,uint param_2,uint param_3);
void ____memsets_change_to_arm(uint *param_1,uint param_2,uint param_3);

