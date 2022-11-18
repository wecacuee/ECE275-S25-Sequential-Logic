`include "./DE0_VGA.v"

module VGA_demo(CLK_50, VGA_BUS_R, VGA_BUS_G, VGA_BUS_B, VGA_HS, VGA_VS);

input	wire			CLK_50;

output	reg	[3:0]		VGA_BUS_R;		//Output Red
output	reg	[3:0]		VGA_BUS_G;		//Output Green
output	reg	[3:0]		VGA_BUS_B;		//Output Blue

output	reg	[0:0]		VGA_HS;			//Horizontal Sync
output	reg	[0:0]		VGA_VS;			//Vertical Sync

reg			[9:0]		X_pix;			//Location in X of the driver
reg			[9:0]		Y_pix;			//Location in Y of the driver

reg			[0:0]		H_visible;		//H_blank?
reg			[0:0]		V_visible;		//V_blank?

wire		[0:0]		pixel_clk;		//Pixel clock. Every clock a pixel is being drawn. 
reg			[9:0]		pixel_cnt;		//How many pixels have been output.

reg			[11:0]		pixel_color;	//12 Bits representing color of pixel, 4 bits for R, G, and B
										//4 bits for Blue are in most significant position, Red in least
									
							
always @(posedge pixel_clk)
	begin
		pixel_color <= 12'b1111_0000_0000;
	end
	
		//Pass pins and current pixel values to display driver
		DE0_VGA VGA_Driver
		(
			.clk_50(CLK_50),
			.pixel_color(pixel_color),
			.VGA_BUS_R(VGA_BUS_R), 
			.VGA_BUS_G(VGA_BUS_G), 
			.VGA_BUS_B(VGA_BUS_B), 
			.VGA_HS(VGA_HS), 
			.VGA_VS(VGA_VS), 
			.X_pix(X_pix), 
			.Y_pix(Y_pix), 
			.H_visible(H_visible),
			.V_visible(V_visible), 
			.pixel_clk(pixel_clk),
			.pixel_cnt(pixel_cnt)
		);

endmodule
