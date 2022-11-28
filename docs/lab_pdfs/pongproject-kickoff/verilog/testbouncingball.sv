`timescale 10ms/1ms
module testbouncingball();
  reg clock;
  reg reset;
  reg [9:0] paddle_y;
  initial begin
    clock = 0;
    paddle_y = 10'd240;
    #5 reset = 1;
    #15 reset = 0;
  end
  always #5 clock = ~clock;
    
  wire [9:0] ballx;
  wire [9:0] bally;
  
  bouncingball bball( clock, reset, paddle_y, ballx, bally);
endmodule
