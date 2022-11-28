`timescale 1ns/100ps
module testslowclock();
  reg fastclock;
  reg reset;
  initial begin
    fastclock = 0;
    reset = 1;
    #10 reset = 0;
  end
  always #10 fastclock = ~fastclock;
    
  wire slowclk;
  slowclock mod( fastclock, reset, slowclk);
endmodule