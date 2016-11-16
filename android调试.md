
android:debuggable="true"

am start -D -n f8left.cm2/f8left.cm2.MainActivity
jdb -connect com.sun.jdi.SocketAttach:hostname=127.0.0.1,port=8700
adb forward tcp:23946 tcp:23946

.text:00000D80 ; signed int __fastcall JNI_OnLoad(JavaVM *vm)
.text:00000D80                 EXPORT JNI_OnLoad
.text:00000D80 JNI_OnLoad
.text:00000D80
.text:00000D80 var_28          = -0x28
.text:00000D80 var_24          = -0x24
.text:00000D80 env             = -0x18
.text:00000D80
.text:00000D80                 PUSH            {R4-R7,LR}
.text:00000D82                 SUB             SP, SP, #0x14
.text:00000D84                 ADD             R1, SP, #0x28+env
.text:00000D86                 MOVS            R3, #0
.text:00000D88                 LDR             R2, =0x10006
.text:00000D8A                 STR.W           R3, [R1,#-0x10]!
.text:00000D8E                 LDR             R3, [R0]
.text:00000D90                 LDR             R3, [R3,#0x18]