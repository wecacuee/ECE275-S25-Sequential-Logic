`timescale 10ms/1ms
module testpaddle();
   reg clock;
   reg reset;
   reg [1:0] up_down; // up down buttons
   wire [9:0] paddle_y; // y coordinate of paddle y
   initial begin
      clock = 0;
      reset = 0;
      up_down = 2'b00;
      #1 reset = 1;
      #2 reset = 0;
      #10 up_down = 2'b10; // after delay of 100ms press up
      #10 up_down = 2'b01; // after delay of 100ms press down
      #10 up_down = 2'b00; // after delay of 100ms release both
   end
   always #1 clock = ~clock; // 20ms clock time period

   paddle pdl( clock, reset, up_down, paddle_y);
endmodule
