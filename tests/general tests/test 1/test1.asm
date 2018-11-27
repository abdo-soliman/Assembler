mOv r0,r1 ;expected output 0xF001
Add (R2)+,r3 ;expected output 0xE283
Or -(R3),R0 ;expected output 0x94C0
CMp 100(r5),R1 ;expected output 0x7741
XnOr #500,r3 ;expected output 0x89C3
sub @r0,r1 ;expected output 0xC801
sbc @(R2)+,r3 ;expected output 0xBA83
adc @-(R3),R0 ;expected output 0xDCC0
and @100(r5),R1 ;expected output 0xAF41
;mov @#500,r3 ;expected output error

