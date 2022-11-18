module slowclock(input wire fastclk,
                 input wire reset,
                 output reg slowclk) ;

   // Register block
   reg [2:0]                count; // State of the D-ffs
   wire [2:0]               d; // Input to the D-ffs
   always_ff @(posedge fastclk or posedge reset)
     begin
        if (reset) begin
           count <= 3'b0;
        end else begin
           count <= d;
        end
     end


   // input + current state -> ff input block
   assign d = (count == 3'b011) ? 3'b000 : count + 3'b001;

   // current state -> output block
   always_comb
     begin
        if (count == 3'b000)
          assign slowclk = 1'b1;
        else
          assign slowclk = 1'b0;
     end

endmodule
