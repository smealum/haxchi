# This stuff may need a change in different kernel versions
# This is only needed when launched directly through browser and not SD card.

.section ".kernel_code"
    .globl SaveAndResetDataBATs_And_SRs_hook
SaveAndResetDataBATs_And_SRs_hook:
    # setup CTR to the position we need to return to
    mflr r5
    mtctr r5
    # set link register to its original value
    mtlr r7
    # setup us a nice DBAT for our code data with same region as our code
    mfspr r5, 560
    mtspr 570, r5
    mfspr r5, 561
    mtspr 571, r5
    # restore the original kernel instructions that we replaced
    lwz r5, 0x34(r3)
    lwz r6, 0x38(r3)
    lwz r7, 0x3C(r3)
    lwz r8, 0x40(r3)
    lwz r9, 0x44(r3)
    lwz r10, 0x48(r3)
    lwz r11, 0x4C(r3)
    lwz r3, 0x50(r3)
    isync
    mtsr 7, r5
    # jump back to the position in kernel after our patch (from LR)
    bctr

.extern my_PrepareTitle
    .globl my_PrepareTitle_hook
my_PrepareTitle_hook:
    # store all registers on stack to avoid issues with the call to C functions
    stwu r1, -0x90(r1)
    # registers for our own usage
    # just store everything
    stmw r3, 0x10(r1)

    # save the LR from where we came
    mflr r31

    # the cos.xml/app.xml structure is at the location 0x68 of r11
    # there are actually many places that can be hooked for it
    # e.g. 0xFFF16130 and r27 points to this structure
    addi r3, r11, 0x68

    bl my_PrepareTitle

    # setup LR to jump back to kernel code
    mtlr r31

    # restore all original values of registers from stack
    lmw r3, 0x10(r1)

    # restore the stack
    addi r1, r1, 0x90

    # restore original instruction that we replaced in the kernel
    clrlwi    r7, r12, 0

    # jump back
    blr

     .globl SC0x25_KernelCopyData
SC0x25_KernelCopyData:
    li r0, 0x2500
    sc
    blr
