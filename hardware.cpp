#include "systemc.h"
#include "digit.h"
#include "hardware.h"


// void m_register::run_m_register()
// {

// }

void splitter::do_splitter()
{
	out_high.write(HIGH_HALF(in.read()));
	out_low.write(LOW_HALF(in.read()));
}

void adder::do_adder()
{
	out.write(in_1.read() + in_2.read());
}

void multiplier::do_multiplier()
{
	out.write(in_1.read() * in_2.read());
}

void multiplexor::do_multiplexor()
{
	if (select.read() == 0)
	{
		out.write(in_1.read());
	}
	else
	{
		out.write(in_2.read());
	}
}

void compare_less_than::do_compare_less_than()
{
	if (in.read() < reference.read())
	{
		out.write(1);
	}
	else
	{
		out.write(0);
	}
}

void m_to_hh::to_hh()
{
	out.write(TO_HIGH_HALF(in.read()));
}

void get_hh::do_get_hh()
{
	out.write(HIGH_HALF(in.read()));
}

