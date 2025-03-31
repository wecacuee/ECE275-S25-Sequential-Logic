// Design a sequential circuit where the output becomes 1 and remains 1 when at least two 0's and at least 1's 
// have occured regardless of their order.
// Example:
// 	X= 0 1 1 1 0 1 0 1 0 1 1 0
// 	Z= 0 0 0 0 0 1 1 1 1 1 1 1
module statespacetop (Clock, Resetn, I, O);

input Clock, Resetn, I;
output logic O = 'b0;

parameter [3:0]
      S0 = 4'b0000, S1 = 4'b0001, 
		S2 = 4'b0010, S3 = 4'b0011,
		S4 = 4'b0100, S6 = 4'b0101,
		S8 = 4'b0110, S9 = 4'b0111,
		S10 = 4'b1000;
		
logic [3:0] D = S0;
logic [3:0] Q = S0;

// Define the next state combinational circuit
always_comb
	case (Q)
	S0: begin
		D = ~I ? S1 : S2;
		O = 'b0;
	end
	S1: begin
		D = ~I ? S3 : S4;
		O = 'b0;
	end
	S2: begin
		D = ~I ? S4 : S6;
		O = 'b0;
	end
	S3: begin
		D = ~I ? S3 : S8;
		O = 'b0;
	end
	S4: begin
		D = ~I ? S8 : S9;
		O = 'b0;
	end
	S6: begin
		D = ~I ? S9 : S6;
		O = 'b0;
	end
	S8: begin
		D = ~I ? S8 : S10;
		O = 'b0;
	end
	S9: begin
		D = ~I ? S10 : S9;
		O = 'b0;
	end
	S10: begin
		D = S10;
		O = 'b0;
	end
	default: 
	begin 
		D = 2'bxx;
		// Define output
		O = 'bx;
	end
	endcase

	
	// Define the sequential block
always @(negedge Resetn, posedge Clock)
	if (Resetn == 0) Q <= S0;
	else Q <= D;
endmodule
