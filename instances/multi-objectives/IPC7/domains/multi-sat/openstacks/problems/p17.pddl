(define (problem os-time-p41_1)
(:domain openstacks-time-nonADL-nonNegated)
(:objects 
n0 n1 n2 n3 n4 n5 n6 n7 n8 n9 n10 n11 n12 n13 n14 n15 n16 n17 n18 n19 n20 n21 n22 n23 n24 n25 n26 n27 n28 n29 n30 n31 n32 n33 n34 n35 n36  - count
)

(:init
(next-count n0 n1) (next-count n1 n2) (next-count n2 n3) (next-count n3 n4) (next-count n4 n5) (next-count n5 n6) (next-count n6 n7) (next-count n7 n8) (next-count n8 n9) (next-count n9 n10) (next-count n10 n11) (next-count n11 n12) (next-count n12 n13) (next-count n13 n14) (next-count n14 n15) (next-count n15 n16) (next-count n16 n17) (next-count n17 n18) (next-count n18 n19) (next-count n19 n20) (next-count n20 n21) (next-count n21 n22) (next-count n22 n23) (next-count n23 n24) (next-count n24 n25) (next-count n25 n26) (next-count n26 n27) (next-count n27 n28) (next-count n28 n29) (next-count n29 n30) (next-count n30 n31) (next-count n31 n32) (next-count n32 n33) (next-count n33 n34) (next-count n34 n35) (next-count n35 n36) 
(stacks-avail n0)
(waiting o1)
(includes o1 p4)(includes o1 p7)(includes o1 p11)(includes o1 p13)(includes o1 p20)(includes o1 p29)

(waiting o2)
(includes o2 p7)(includes o2 p8)(includes o2 p17)

(waiting o3)
(includes o3 p23)(includes o3 p36)(includes o3 p40)

(waiting o4)
(includes o4 p40)

(waiting o5)
(includes o5 p4)(includes o5 p6)(includes o5 p8)(includes o5 p21)

(waiting o6)
(includes o6 p6)(includes o6 p12)(includes o6 p14)(includes o6 p18)(includes o6 p19)(includes o6 p21)(includes o6 p33)(includes o6 p39)

(waiting o7)
(includes o7 p40)

(waiting o8)
(includes o8 p1)(includes o8 p25)(includes o8 p28)(includes o8 p34)(includes o8 p37)

(waiting o9)
(includes o9 p3)(includes o9 p7)(includes o9 p38)(includes o9 p39)

(waiting o10)
(includes o10 p5)(includes o10 p7)(includes o10 p28)(includes o10 p37)

(waiting o11)
(includes o11 p35)

(waiting o12)
(includes o12 p1)(includes o12 p12)(includes o12 p33)(includes o12 p34)(includes o12 p37)

(waiting o13)
(includes o13 p23)(includes o13 p24)

(waiting o14)
(includes o14 p22)(includes o14 p37)

(waiting o15)
(includes o15 p2)(includes o15 p19)(includes o15 p28)(includes o15 p30)

(waiting o16)
(includes o16 p9)(includes o16 p13)(includes o16 p32)

(waiting o17)
(includes o17 p4)(includes o17 p6)(includes o17 p17)(includes o17 p18)(includes o17 p24)(includes o17 p32)(includes o17 p41)

(waiting o18)
(includes o18 p1)(includes o18 p3)(includes o18 p4)

(waiting o19)
(includes o19 p1)(includes o19 p3)(includes o19 p10)(includes o19 p27)(includes o19 p30)(includes o19 p34)

(waiting o20)
(includes o20 p15)

(waiting o21)
(includes o21 p15)(includes o21 p41)

(waiting o22)
(includes o22 p1)(includes o22 p22)(includes o22 p30)

(waiting o23)
(includes o23 p16)(includes o23 p28)(includes o23 p30)

(waiting o24)
(includes o24 p10)(includes o24 p13)(includes o24 p20)(includes o24 p31)(includes o24 p36)

(waiting o25)
(includes o25 p17)(includes o25 p40)

(waiting o26)
(includes o26 p3)(includes o26 p14)(includes o26 p24)(includes o26 p32)

(waiting o27)
(includes o27 p3)(includes o27 p24)(includes o27 p38)

(waiting o28)
(includes o28 p24)(includes o28 p31)

(waiting o29)
(includes o29 p13)(includes o29 p23)

(waiting o30)
(includes o30 p8)(includes o30 p24)(includes o30 p41)

(waiting o31)
(includes o31 p22)

(waiting o32)
(includes o32 p8)(includes o32 p15)(includes o32 p18)(includes o32 p20)(includes o32 p38)(includes o32 p39)

(waiting o33)
(includes o33 p12)

(waiting o34)
(includes o34 p3)(includes o34 p25)(includes o34 p26)(includes o34 p33)(includes o34 p37)(includes o34 p39)

(waiting o35)
(includes o35 p7)(includes o35 p10)(includes o35 p33)

(waiting o36)
(includes o36 p10)(includes o36 p14)(includes o36 p17)(includes o36 p28)

(waiting o37)
(includes o37 p18)(includes o37 p23)(includes o37 p24)(includes o37 p31)

(waiting o38)
(includes o38 p4)(includes o38 p17)(includes o38 p38)(includes o38 p40)

(waiting o39)
(includes o39 p18)(includes o39 p31)(includes o39 p40)

(waiting o40)
(includes o40 p1)(includes o40 p13)(includes o40 p17)(includes o40 p19)(includes o40 p21)(includes o40 p26)(includes o40 p27)(includes o40 p37)

(waiting o41)
(includes o41 p5)(includes o41 p16)

(not-made p1)
(not-made p2)
(not-made p3)
(not-made p4)
(not-made p5)
(not-made p6)
(not-made p7)
(not-made p8)
(not-made p9)
(not-made p10)
(not-made p11)
(not-made p12)
(not-made p13)
(not-made p14)
(not-made p15)
(not-made p16)
(not-made p17)
(not-made p18)
(not-made p19)
(not-made p20)
(not-made p21)
(not-made p22)
(not-made p23)
(not-made p24)
(not-made p25)
(not-made p26)
(not-made p27)
(not-made p28)
(not-made p29)
(not-made p30)
(not-made p31)
(not-made p32)
(not-made p33)
(not-made p34)
(not-made p35)
(not-made p36)
(not-made p37)
(not-made p38)
(not-made p39)
(not-made p40)
(not-made p41)
)

(:goal
(and
(shipped o1)
(shipped o2)
(shipped o3)
(shipped o4)
(shipped o5)
(shipped o6)
(shipped o7)
(shipped o8)
(shipped o9)
(shipped o10)
(shipped o11)
(shipped o12)
(shipped o13)
(shipped o14)
(shipped o15)
(shipped o16)
(shipped o17)
(shipped o18)
(shipped o19)
(shipped o20)
(shipped o21)
(shipped o22)
(shipped o23)
(shipped o24)
(shipped o25)
(shipped o26)
(shipped o27)
(shipped o28)
(shipped o29)
(shipped o30)
(shipped o31)
(shipped o32)
(shipped o33)
(shipped o34)
(shipped o35)
(shipped o36)
(shipped o37)
(shipped o38)
(shipped o39)
(shipped o40)
(shipped o41)
))

(:metric (and (minimize (total-time)) (minimize (total-cost))))

)

