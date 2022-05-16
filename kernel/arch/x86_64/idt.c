#include "levi/arch/x86_64/idt.h"

#include <levi/arch/x86_64/cpuregs.h>
#include <levi/arch/x86_64/gdt.h>
#include <levi/interrupts/interrupts.h>
#include <levi/memory/vmm.h>
#include <levi/panic.h>
#include <levi/proc/process.h>
#include <levi/proc/scheduler.h>
#include <levi/utils/string.h>

static struct idt_64_entry idt_entries[0xFF] = { 0 };
static struct idt_64_ptr idt_ptr = { sizeof(idt_entries), (u64)idt_entries };

static const char exceptions_message[32][32] = {
    "Divide-by-zero Error",
    "Debug",
    "Non-maskable Interrupt",
    "Breakpoint",
    "Overflow",
    "Bound Range Exceeded",
    "Invalid Opcode",
    "Device Not Available",
    "Double Fault",
    "Coprocessor Segment Overrun",
    "Invalid TSS",
    "Segment Not Present",
    "Stack-Segment Fault",
    "General Protection Fault",
    "Page Fault",
    "Reserved",
    "x87 Floating-Point Exception",
    "Alignment Check",
    "Machine Check",
    "SIMD Floating-Point Exception",
    "Virtualization Exception",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved"
};

static void set_entry(u32 index, u64 address, u16 selector, u8 ist,
                      u8 type_attr)
{
    idt_entries[index].offset_1 = address & 0xFFFF;
    idt_entries[index].offset_2 = (address >> 16) & 0xFFFF;
    idt_entries[index].offset_3 = (address >> 32) & 0xFFFFFFFF;
    idt_entries[index].selector = selector;
    idt_entries[index].ist = ist;
    idt_entries[index].type_attr = type_attr;
    idt_entries[index].zero = 0x0;
}

void interrupts_disable()
{
    asm volatile("cli");
}

void interrupts_enable()
{
    asm volatile("sti");
}

static void handle_exception(u64 index, u64 error_code, proc_t *proc)
{
    term_print("PANIC: %s\n", exceptions_message[index]);

    term_print("error code %lx saved rip: 0x%lx cs: %lx\n", error_code,
               proc->ctx.rip, proc->ctx.cs);

    switch (index)
    {
    case 0xE: {
        u64 cr2 = cr2_read();
        term_print("cr2: %lx\n", cr2);

        u64 phys_addrr = 0x0;
        vas_t curr_vas = proc->vas;

        if (vma_to_phys(&curr_vas, cr2, &phys_addrr) == MAP_FAILED)
        {
            term_print("Page not present\n");
        }
        else
        {
            term_print("%lx -> %lx\n", cr2, phys_addrr);
        }
        break;
    }
    default:
        break;
    }
    die();
}

void __isr_c_handler(context_t *ctx)
{
    // Saving the context

    proc_t *proc = proc_get(sched_get());

    if (proc == NULL)
    {
        die();
    }

    memcpy(&proc->ctx, ctx, sizeof(context_t));

    throw_interrupts(ctx->index, ctx->error_code, proc);

    proc = proc_get(sched_get());

    // Restore process context
    memcpy(ctx, &proc->ctx, sizeof(context_t));
}

extern void isr_0(void);
extern void isr_1(void);
extern void isr_2(void);
extern void isr_3(void);
extern void isr_4(void);
extern void isr_5(void);
extern void isr_6(void);
extern void isr_7(void);
extern void isr_8(void);
extern void isr_9(void);
extern void isr_10(void);
extern void isr_11(void);
extern void isr_12(void);
extern void isr_13(void);
extern void isr_14(void);
extern void isr_15(void);
extern void isr_16(void);
extern void isr_17(void);
extern void isr_18(void);
extern void isr_19(void);
extern void isr_20(void);
extern void isr_21(void);
extern void isr_22(void);
extern void isr_23(void);
extern void isr_24(void);
extern void isr_25(void);
extern void isr_26(void);
extern void isr_27(void);
extern void isr_28(void);
extern void isr_29(void);
extern void isr_30(void);
extern void isr_31(void);
extern void isr_32(void);

// Generated code
extern void isr_33(void);
extern void isr_34(void);
extern void isr_35(void);
extern void isr_36(void);
extern void isr_37(void);
extern void isr_38(void);
extern void isr_39(void);
extern void isr_40(void);
extern void isr_41(void);
extern void isr_42(void);
extern void isr_43(void);
extern void isr_44(void);
extern void isr_45(void);
extern void isr_46(void);
extern void isr_47(void);
extern void isr_48(void);
extern void isr_49(void);
extern void isr_50(void);
extern void isr_51(void);
extern void isr_52(void);
extern void isr_53(void);
extern void isr_54(void);
extern void isr_55(void);
extern void isr_56(void);
extern void isr_57(void);
extern void isr_58(void);
extern void isr_59(void);
extern void isr_60(void);
extern void isr_61(void);
extern void isr_62(void);
extern void isr_63(void);
extern void isr_64(void);
extern void isr_65(void);
extern void isr_66(void);
extern void isr_67(void);
extern void isr_68(void);
extern void isr_69(void);
extern void isr_70(void);
extern void isr_71(void);
extern void isr_72(void);
extern void isr_73(void);
extern void isr_74(void);
extern void isr_75(void);
extern void isr_76(void);
extern void isr_77(void);
extern void isr_78(void);
extern void isr_79(void);
extern void isr_80(void);
extern void isr_81(void);
extern void isr_82(void);
extern void isr_83(void);
extern void isr_84(void);
extern void isr_85(void);
extern void isr_86(void);
extern void isr_87(void);
extern void isr_88(void);
extern void isr_89(void);
extern void isr_90(void);
extern void isr_91(void);
extern void isr_92(void);
extern void isr_93(void);
extern void isr_94(void);
extern void isr_95(void);
extern void isr_96(void);
extern void isr_97(void);
extern void isr_98(void);
extern void isr_99(void);
extern void isr_100(void);
extern void isr_101(void);
extern void isr_102(void);
extern void isr_103(void);
extern void isr_104(void);
extern void isr_105(void);
extern void isr_106(void);
extern void isr_107(void);
extern void isr_108(void);
extern void isr_109(void);
extern void isr_110(void);
extern void isr_111(void);
extern void isr_112(void);
extern void isr_113(void);
extern void isr_114(void);
extern void isr_115(void);
extern void isr_116(void);
extern void isr_117(void);
extern void isr_118(void);
extern void isr_119(void);
extern void isr_120(void);
extern void isr_121(void);
extern void isr_122(void);
extern void isr_123(void);
extern void isr_124(void);
extern void isr_125(void);
extern void isr_126(void);
extern void isr_127(void);
extern void isr_128(void);
extern void isr_129(void);
extern void isr_130(void);
extern void isr_131(void);
extern void isr_132(void);
extern void isr_133(void);
extern void isr_134(void);
extern void isr_135(void);
extern void isr_136(void);
extern void isr_137(void);
extern void isr_138(void);
extern void isr_139(void);
extern void isr_140(void);
extern void isr_141(void);
extern void isr_142(void);
extern void isr_143(void);
extern void isr_144(void);
extern void isr_145(void);
extern void isr_146(void);
extern void isr_147(void);
extern void isr_148(void);
extern void isr_149(void);
extern void isr_150(void);
extern void isr_151(void);
extern void isr_152(void);
extern void isr_153(void);
extern void isr_154(void);
extern void isr_155(void);
extern void isr_156(void);
extern void isr_157(void);
extern void isr_158(void);
extern void isr_159(void);
extern void isr_160(void);
extern void isr_161(void);
extern void isr_162(void);
extern void isr_163(void);
extern void isr_164(void);
extern void isr_165(void);
extern void isr_166(void);
extern void isr_167(void);
extern void isr_168(void);
extern void isr_169(void);
extern void isr_170(void);
extern void isr_171(void);
extern void isr_172(void);
extern void isr_173(void);
extern void isr_174(void);
extern void isr_175(void);
extern void isr_176(void);
extern void isr_177(void);
extern void isr_178(void);
extern void isr_179(void);
extern void isr_180(void);
extern void isr_181(void);
extern void isr_182(void);
extern void isr_183(void);
extern void isr_184(void);
extern void isr_185(void);
extern void isr_186(void);
extern void isr_187(void);
extern void isr_188(void);
extern void isr_189(void);
extern void isr_190(void);
extern void isr_191(void);
extern void isr_192(void);
extern void isr_193(void);
extern void isr_194(void);
extern void isr_195(void);
extern void isr_196(void);
extern void isr_197(void);
extern void isr_198(void);
extern void isr_199(void);
extern void isr_200(void);
extern void isr_201(void);
extern void isr_202(void);
extern void isr_203(void);
extern void isr_204(void);
extern void isr_205(void);
extern void isr_206(void);
extern void isr_207(void);
extern void isr_208(void);
extern void isr_209(void);
extern void isr_210(void);
extern void isr_211(void);
extern void isr_212(void);
extern void isr_213(void);
extern void isr_214(void);
extern void isr_215(void);
extern void isr_216(void);
extern void isr_217(void);
extern void isr_218(void);
extern void isr_219(void);
extern void isr_220(void);
extern void isr_221(void);
extern void isr_222(void);
extern void isr_223(void);
extern void isr_224(void);
extern void isr_225(void);
extern void isr_226(void);
extern void isr_227(void);
extern void isr_228(void);
extern void isr_229(void);
extern void isr_230(void);
extern void isr_231(void);
extern void isr_232(void);
extern void isr_233(void);
extern void isr_234(void);
extern void isr_235(void);
extern void isr_236(void);
extern void isr_237(void);
extern void isr_238(void);
extern void isr_239(void);
extern void isr_240(void);
extern void isr_241(void);
extern void isr_242(void);
extern void isr_243(void);
extern void isr_244(void);
extern void isr_245(void);
extern void isr_246(void);
extern void isr_247(void);
extern void isr_248(void);
extern void isr_249(void);
extern void isr_250(void);
extern void isr_251(void);
extern void isr_252(void);
extern void isr_253(void);
extern void isr_254(void);

void idt_init()
{
    set_entry(0, (u64)isr_0, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(1, (u64)isr_1, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(2, (u64)isr_2, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(3, (u64)isr_3, SEGMENT_SELECTOR(5, 0, 0), 0, TRAP_GATE);
    set_entry(4, (u64)isr_4, SEGMENT_SELECTOR(5, 0, 0), 0, TRAP_GATE);
    set_entry(5, (u64)isr_5, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(6, (u64)isr_6, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(7, (u64)isr_7, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(8, (u64)isr_8, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(9, (u64)isr_9, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(10, (u64)isr_10, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(11, (u64)isr_11, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(12, (u64)isr_12, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(13, (u64)isr_13, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(14, (u64)isr_14, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(15, (u64)isr_15, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(16, (u64)isr_16, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(17, (u64)isr_17, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(18, (u64)isr_18, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(19, (u64)isr_19, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(20, (u64)isr_20, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(21, (u64)isr_21, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(22, (u64)isr_22, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(23, (u64)isr_23, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(24, (u64)isr_24, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(25, (u64)isr_25, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(26, (u64)isr_26, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(27, (u64)isr_27, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(28, (u64)isr_28, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(29, (u64)isr_29, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(30, (u64)isr_30, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(31, (u64)isr_31, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(32, (u64)isr_32, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(33, (u64)isr_33, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);

    // generated code
    set_entry(34, (u64)isr_34, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(35, (u64)isr_35, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(36, (u64)isr_36, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(37, (u64)isr_37, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(38, (u64)isr_38, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(39, (u64)isr_39, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(40, (u64)isr_40, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(41, (u64)isr_41, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(42, (u64)isr_42, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(43, (u64)isr_43, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(44, (u64)isr_44, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(45, (u64)isr_45, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(46, (u64)isr_46, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(47, (u64)isr_47, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(48, (u64)isr_48, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(49, (u64)isr_49, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(50, (u64)isr_50, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(51, (u64)isr_51, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(52, (u64)isr_52, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(53, (u64)isr_53, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(54, (u64)isr_54, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(55, (u64)isr_55, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(56, (u64)isr_56, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(57, (u64)isr_57, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(58, (u64)isr_58, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(59, (u64)isr_59, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(60, (u64)isr_60, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(61, (u64)isr_61, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(62, (u64)isr_62, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(63, (u64)isr_63, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(64, (u64)isr_64, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(65, (u64)isr_65, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(66, (u64)isr_66, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(67, (u64)isr_67, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(68, (u64)isr_68, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(69, (u64)isr_69, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(70, (u64)isr_70, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(71, (u64)isr_71, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(72, (u64)isr_72, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(73, (u64)isr_73, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(74, (u64)isr_74, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(75, (u64)isr_75, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(76, (u64)isr_76, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(77, (u64)isr_77, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(78, (u64)isr_78, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(79, (u64)isr_79, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(80, (u64)isr_80, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(81, (u64)isr_81, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(82, (u64)isr_82, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(83, (u64)isr_83, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(84, (u64)isr_84, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(85, (u64)isr_85, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(86, (u64)isr_86, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(87, (u64)isr_87, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(88, (u64)isr_88, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(89, (u64)isr_89, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(90, (u64)isr_90, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(91, (u64)isr_91, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(92, (u64)isr_92, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(93, (u64)isr_93, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(94, (u64)isr_94, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(95, (u64)isr_95, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(96, (u64)isr_96, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(97, (u64)isr_97, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(98, (u64)isr_98, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(99, (u64)isr_99, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(100, (u64)isr_100, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(101, (u64)isr_101, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(102, (u64)isr_102, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(103, (u64)isr_103, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(104, (u64)isr_104, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(105, (u64)isr_105, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(106, (u64)isr_106, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(107, (u64)isr_107, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(108, (u64)isr_108, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(109, (u64)isr_109, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(110, (u64)isr_110, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(111, (u64)isr_111, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(112, (u64)isr_112, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(113, (u64)isr_113, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(114, (u64)isr_114, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(115, (u64)isr_115, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(116, (u64)isr_116, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(117, (u64)isr_117, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(118, (u64)isr_118, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(119, (u64)isr_119, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(120, (u64)isr_120, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(121, (u64)isr_121, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(122, (u64)isr_122, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(123, (u64)isr_123, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(124, (u64)isr_124, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(125, (u64)isr_125, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(126, (u64)isr_126, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(127, (u64)isr_127, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(128, (u64)isr_128, SEGMENT_SELECTOR(5, 0, 0), 0,
              INTERRUPT_GATE | ALLOW_TO_USER); // Gate used for syscall
    set_entry(129, (u64)isr_129, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(130, (u64)isr_130, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(131, (u64)isr_131, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(132, (u64)isr_132, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(133, (u64)isr_133, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(134, (u64)isr_134, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(135, (u64)isr_135, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(136, (u64)isr_136, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(137, (u64)isr_137, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(138, (u64)isr_138, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(139, (u64)isr_139, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(140, (u64)isr_140, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(141, (u64)isr_141, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(142, (u64)isr_142, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(143, (u64)isr_143, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(144, (u64)isr_144, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(145, (u64)isr_145, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(146, (u64)isr_146, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(147, (u64)isr_147, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(148, (u64)isr_148, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(149, (u64)isr_149, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(150, (u64)isr_150, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(151, (u64)isr_151, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(152, (u64)isr_152, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(153, (u64)isr_153, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(154, (u64)isr_154, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(155, (u64)isr_155, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(156, (u64)isr_156, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(157, (u64)isr_157, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(158, (u64)isr_158, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(159, (u64)isr_159, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(160, (u64)isr_160, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(161, (u64)isr_161, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(162, (u64)isr_162, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(163, (u64)isr_163, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(164, (u64)isr_164, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(165, (u64)isr_165, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(166, (u64)isr_166, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(167, (u64)isr_167, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(168, (u64)isr_168, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(169, (u64)isr_169, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(170, (u64)isr_170, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(171, (u64)isr_171, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(172, (u64)isr_172, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(173, (u64)isr_173, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(174, (u64)isr_174, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(175, (u64)isr_175, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(176, (u64)isr_176, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(177, (u64)isr_177, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(178, (u64)isr_178, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(179, (u64)isr_179, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(180, (u64)isr_180, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(181, (u64)isr_181, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(182, (u64)isr_182, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(183, (u64)isr_183, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(184, (u64)isr_184, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(185, (u64)isr_185, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(186, (u64)isr_186, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(187, (u64)isr_187, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(188, (u64)isr_188, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(189, (u64)isr_189, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(190, (u64)isr_190, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(191, (u64)isr_191, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(192, (u64)isr_192, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(193, (u64)isr_193, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(194, (u64)isr_194, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(195, (u64)isr_195, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(196, (u64)isr_196, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(197, (u64)isr_197, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(198, (u64)isr_198, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(199, (u64)isr_199, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(200, (u64)isr_200, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(201, (u64)isr_201, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(202, (u64)isr_202, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(203, (u64)isr_203, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(204, (u64)isr_204, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(205, (u64)isr_205, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(206, (u64)isr_206, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(207, (u64)isr_207, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(208, (u64)isr_208, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(209, (u64)isr_209, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(210, (u64)isr_210, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(211, (u64)isr_211, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(212, (u64)isr_212, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(213, (u64)isr_213, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(214, (u64)isr_214, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(215, (u64)isr_215, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(216, (u64)isr_216, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(217, (u64)isr_217, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(218, (u64)isr_218, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(219, (u64)isr_219, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(220, (u64)isr_220, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(221, (u64)isr_221, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(222, (u64)isr_222, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(223, (u64)isr_223, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(224, (u64)isr_224, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(225, (u64)isr_225, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(226, (u64)isr_226, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(227, (u64)isr_227, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(228, (u64)isr_228, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(229, (u64)isr_229, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(230, (u64)isr_230, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(231, (u64)isr_231, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(232, (u64)isr_232, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(233, (u64)isr_233, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(234, (u64)isr_234, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(235, (u64)isr_235, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(236, (u64)isr_236, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(237, (u64)isr_237, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(238, (u64)isr_238, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(239, (u64)isr_239, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(240, (u64)isr_240, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(241, (u64)isr_241, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(242, (u64)isr_242, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(243, (u64)isr_243, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(244, (u64)isr_244, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(245, (u64)isr_245, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(246, (u64)isr_246, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(247, (u64)isr_247, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(248, (u64)isr_248, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(249, (u64)isr_249, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(250, (u64)isr_250, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(251, (u64)isr_251, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(252, (u64)isr_252, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(253, (u64)isr_253, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);
    set_entry(254, (u64)isr_254, SEGMENT_SELECTOR(5, 0, 0), 0, INTERRUPT_GATE);

    asm volatile("lidt %0\n\t" ::"m"(idt_ptr));

    // Register exceptions
    for (u32 i = 0; i < 32; ++i)
    {
        register_interrupt_handler(i, handle_exception);
    }
}