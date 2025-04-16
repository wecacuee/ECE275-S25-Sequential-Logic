`include "./DE0_VGA.v"

module VGA_top(CLOCK_50, 
                VGA_R, VGA_G, VGA_B, VGA_HS, VGA_VS);

input	wire			CLOCK_50;

output	wire	[3:0]		VGA_R;		//Output Red
output	wire	[3:0]		VGA_G;		//Output Green
output	wire	[3:0]		VGA_B;		//Output Blue

output	wire	[0:0]		VGA_HS;			//Horizontal Sync
output	wire	[0:0]		VGA_VS;			//Vertical Sync

wire			[9:0]		X_pix;			//Location in X of the driver
wire			[9:0]		Y_pix;			//Location in Y of the driver

wire			[0:0]		H_visible;		//H_blank?
wire			[0:0]		V_visible;		//V_blank?

wire		[0:0]		pixel_clk;		//Pixel clock. Every clock a pixel is being drawn. 
wire			[9:0]		pixel_cnt;		//How many pixels have been output.

reg			[11:0]		pixel_color;	//12 Bits representing color of pixel, 4 bits for R, G, and B
										//4 bits for Blue are in most significant position, Red in least
									
	
		//Pass pins and current pixel values to display driver
		DE0_VGA VGA_Driver
		(
			.clk_50(CLOCK_50),
			.pixel_color(pixel_color),
			.VGA_BUS_R(VGA_R), 
			.VGA_BUS_G(VGA_G), 
			.VGA_BUS_B(VGA_B), 
			.VGA_HS(VGA_HS), 
			.VGA_VS(VGA_VS), 
			.X_pix(X_pix), 
			.Y_pix(Y_pix), 
			.H_visible(H_visible),
			.V_visible(V_visible), 
			.pixel_clk(pixel_clk),
			.pixel_cnt(pixel_cnt)
		);
		makebox(.pixel_clk(pixel_clk),
			     .X_pix(X_pix),
				  .Y_pix(Y_pix),
				  .pixel_color(pixel_color));
									
endmodule

module makebox(input wire pixel_clk, 
					input wire [9:0] X_pix, 
					input wire [9:0] Y_pix,
					output wire [11:0] pixel_color);
	always @(posedge pixel_clk)
	begin
		if (400 <= X_pix && X_pix < 450 && 
			 300 <= Y_pix && Y_pix < 350)
			pixel_color <= 12'b0000_1111_0000;
		else
			pixel_color <= 12'b0000_0000_0000;
	end

endmodule
