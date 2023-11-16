module seqdetector_top(
input clock,
input reset,
input X,
output reg Z
);

reg [2:0] state;
reg [2:0] next_state;

// Define states as constants 
parameter [2:0]
S0 = 3'b000,
S1 = 3'b001,
S2 = 3'b010,
S3 = 3'b011,
S4 = 3'b100,
S5 = 3'b101,
S6 = 3'b110;


// Register Block 
always_ff @(posedge clock or posedge reset) begin
	if (reset)
		state <= S0;
	else
		state <= next_state;
end

// Next state block
always_comb begin
	// case statement is like if else but the condition is on a single variable
	case (state) 
		S0:
			next_state <= X ? S0 : S1;
		S1: 
			next_state <= X ? S0 : S2;
		S2: 
			next_state <= X ? S4 : S3;
		S3: 
			next_state <= X ? S5 : S3;
		S4:
			next_state <= X ? S0 : S6;
		S5:
			next_state <= X ? S0 : S6;
		S6:
			next_state <= X ? S0 : S2;
	endcase
end

// Output block (Moore)
always_comb begin
	case (state)
		S5: Z = 1'b1;
		S6: Z = 1'b1;
		default: Z= 1'b0;
	endcase
end
endmodule
