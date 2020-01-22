#pragma once

#include "util/io.h"

namespace serial{

  static inline bool is_transmitter_ready(io_t baseport){
	uint8_t a = io::read8(baseport,5) ;
	if (a & (1<<5) == 32){
		return true;
	}
	return false;

    //insert your code here
  }

  static inline void writechar(uint8_t c, io_t baseport){
	io::write8(baseport,0,c);
    //insert your code here
  }

} //end serial
