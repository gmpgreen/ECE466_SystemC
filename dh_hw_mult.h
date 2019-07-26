#include "systemc.h"
#include "digit.h"
#include "hardware.h"

#ifndef _DH_HW_MULT_H_
#define _DH_HW_MULT_H_ 1

SC_MODULE (dh_hw_mult)
{
  sc_in<bool> hw_mult_enable; 
  sc_in<NN_DIGIT> in_data_1;
  sc_in<NN_DIGIT> in_data_2;
  sc_out<NN_DIGIT> out_data_low;
  sc_out<NN_DIGIT> out_data_high;
  sc_out<bool> hw_mult_done;
  sc_in_clk clk;

  void process_hw_mult();

  // Signals
  sc_signal<NN_DIGIT> bReg, cReg,
                      bLow, bHigh, cLow, cHigh,
                      a0_mult, u_mult, t_mult, a1_mult,
                      u_t_mult, a1_mult_1H,
                      u2, t2, m1, 
                      a0_mult_u2, m1_1,
                      m2, m2_t2,
                      hh_one, one,
                      a0_sig, a1_sig;   

  sc_signal<bool> ldra0, ldra1, ldrb, ldrc, c1, c2;

  //HW Devices
  m_register a0_reg, a1_reg, b_reg, c_reg;

  multiplier a0_multiplier, a1_multiplier, u_multiplier, t_multiplier; 

  adder adder0, adder1, adder2, adder3, adder4;

  compare_less_than comp1, comp2;

  m_to_hh u2_hh;

  multiplexor multip1, multip2;

  splitter split1, split2;

  get_hh hh_t2;

  
  
  SC_CTOR (dh_hw_mult):
  a0_reg("a0_reg"), a1_reg("a1_reg"), b_reg("b_reg"), c_reg("c_reg"),
  a0_multiplier("a0_mult"), a1_multiplier("a1_multiplier"), u_multiplier("u_multiplier"), t_multiplier("t_multiplier"),
  adder0("adder0"), adder1("adder1"), adder2("adder2"), adder3("adder3"), adder4("adder4"),
  comp1("comp1"), comp2("comp2"),
  u2_hh("u2_hh"),
  multip1("multip1"), multip2("multip2"),
  split1("split1"), split2("split2"),
  hh_t2("hh_t2")
  {
    // Register Inputs
    b_reg.clk(clk), b_reg.in(in_data_1), b_reg.load_reg(ldrb), b_reg.out(bReg);
    c_reg.clk(clk), c_reg.in(in_data_2), c_reg.load_reg(ldrb), c_reg.out(cReg);


    // Connect multiplier inputs
    split1.in(bReg), split1.out_high(bHigh), split1.out_low(bLow);
    split2.in(cReg), split2.out_high(cHigh), split2.out_low(cLow);
    
    
    // Multipliers
    a0_multiplier.in_1(bLow), a0_multiplier.in_2(cLow), a0_multiplier.out(a0_mult);
    u_multiplier.in_1(bHigh), u_multiplier.in_2(cLow), u_multiplier.out(u_mult);
    t_multiplier.in_1(bLow), t_multiplier.in_2(cHigh), t_multiplier.out(t_mult);
    a1_multiplier.in_1(bHigh), a1_multiplier.in_2(cHigh), a1_multiplier.out(a1_mult);

    // HighHalf(1)
    hh_one.write(0x00010000);
    // One 
    one.write(0x1);
    
    // Adders
    adder0.in_1(u_mult), adder0.in_2(t_mult), adder0.out(u_t_mult); 
    adder1.in_1(a1_mult), adder1.in_2(hh_one), adder1.out(a1_mult_1H);
    adder2.in_1(a0_mult), adder2.in_2(u2), adder2.out(a0_mult_u2); 
    adder3.in_1(m1), adder3.in_2(one), adder3.out(m1_1);
    adder4.in_1(t2), adder4.in_2(m2), adder4.out(m2_t2);

    // to_HH
    u2_hh.in(u_t_mult), u2_hh.out(u2);

    // get hh
    hh_t2.in(u_t_mult), hh_t2.out(t2);

    // Compares
    comp1.in(u_t_mult), comp1.reference(u_mult), comp1.out(c1);
    comp2.in(a0_mult_u2), comp2.reference(u2), comp2.out(c2);

    // Multiplex
    multip1.in_1(a1_mult), multip1.in_2(a1_mult_1H), multip1.select(c1), multip1.out(m1);
    multip2.in_1(m1), multip2.in_2(m1_1), multip2.select(c2), multip2.out(m2);

    // Output
    a0_reg.clk(clk), a0_reg.in(a0_mult_u2), a0_reg.load_reg(ldra0), a0_reg.out(a0_sig);
    a1_reg.clk(clk), a1_reg.in(m2_t2), a1_reg.load_reg(ldra1), a1_reg.out(a1_sig);



    SC_CTHREAD (process_hw_mult, clk.pos());
  }
  
};

#endif /* end _DH_HW_MULT_H_ */