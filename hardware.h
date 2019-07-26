#include "systemc.h"
#include "digit.h"

#ifndef _HARDWARE_H_
#define _HARDWARE_H_ 1


SC_MODULE (m_register)
{
	sc_in<NN_DIGIT> in;
	sc_in<bool> load_reg;
	sc_out<NN_DIGIT> out;
    sc_in_clk clk;

	void do_register()
    {
        while(true)
	    {
            if (load_reg.read() == 1)
            {	
                out.write(in.read());
            }
            
            wait();
	    }
    }

	SC_CTOR (m_register)
	{
		SC_CTHREAD(do_register, clk.pos());
		out.initialize(0);
	}
};

SC_MODULE (multiplexor)
{
	sc_in<NN_DIGIT> in_1;
	sc_in<NN_DIGIT> in_2;
	sc_out<NN_DIGIT> out;
	sc_in<bool> select;
	
	void do_multiplexor();

	SC_CTOR (multiplexor)
	{
		SC_METHOD(do_multiplexor);
		sensitive << in_1 << in_2 << select;
	}
};

SC_MODULE (splitter)
{
	sc_in<NN_DIGIT> in;
	sc_out<NN_DIGIT> out_high;
	sc_out<NN_DIGIT> out_low;
	
	void do_splitter();

	SC_CTOR (splitter)
	{
		SC_METHOD(do_splitter);
		sensitive << in;
	}
};

SC_MODULE (multiplier)
{
	sc_in<NN_DIGIT> in_1;
	sc_in<NN_DIGIT> in_2;
	sc_out<NN_DIGIT> out;
	
	void do_multiplier();

	SC_CTOR (multiplier)
	{
		SC_METHOD(do_multiplier);
		sensitive << in_1 << in_2;
	}
};

SC_MODULE (adder)
{
	sc_in<NN_DIGIT> in_1;
	sc_in<NN_DIGIT> in_2;
	sc_out<NN_DIGIT> out;
	
	void do_adder();

	SC_CTOR (adder)
	{
		SC_METHOD(do_adder);
		sensitive << in_1 << in_2;
	}
};



SC_MODULE (m_to_hh)
{
	sc_in<NN_DIGIT> in;
	sc_out<NN_DIGIT> out;
	
	void to_hh();

	SC_CTOR (m_to_hh)
	{
		SC_METHOD(to_hh);
		sensitive << in;
	}
};

SC_MODULE (compare_less_than)
{
	sc_in<NN_DIGIT> in;
	sc_in<NN_DIGIT> reference;
	sc_out<bool> out;
	
	void do_compare_less_than();

	SC_CTOR (compare_less_than)
	{
		SC_METHOD(do_compare_less_than);
		sensitive << in << reference;
	}
};

SC_MODULE (get_hh)
{
	sc_in<NN_DIGIT> in;
	sc_out<NN_DIGIT> out;
	
	void do_get_hh();

	SC_CTOR (get_hh)
	{
		SC_METHOD(do_get_hh);
		sensitive << in;
	}
};

#endif 