module dlatchtop(
	input wire D,
	input wire CLK,
	output logic Q
);
  always_latch begin
    if (CLK) 
      Q = D;
    else
      Q = Q;
  end
endmodule

`timescale 10ps/1ps // period/precision
module test_dlatchtop();
  logic D = 0;
  logic CLK = 1;
  logic Q;
  always begin // non synthesizable 
    #10 D = ~D;
  end
  dlatchtop d1(.D(D), .CLK(CLK), .Q(Q));
endmodule


//module srlatchtop(
//	input logic S,
//	input logic R,
//	output logic Q
//);
//	dlatch d1(.d(S + ~R), 
//				 .ena(S ^ R), 
//				 .q(Q));
//endmodule


//module srlatchtop(
//	input logic S,
//	input logic R,
//	output logic Q
//);
//	always_latch begin
//		case ({S, R}) 
//			2'b00: Q = Q;
//			2'b10: Q = 1;
//			2'b01: Q = 0;
//			default: Q = Q;
//		endcase
//	end
//endmodule

// // Cannot connect not gates in a loop
//module srlatchtop(
//	input logic A,
//	output logic B
//);
//	assign B = ~A;
//	assign A = ~B;
//endmodule

//`timescale 1ns/1ps
// module testbench();

//	logic S = 1;
//	logic R = 0;
//	logic Q;
//	initial begin
//		#10 S = 0;
//	end
//	srlatchtop m1(S, R, Q);
// endmodule