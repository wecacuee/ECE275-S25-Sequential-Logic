---
layout: page
---
# Lab notes

- Aug 28, 2022 : [Quartus Setup](https://www.overleaf.com/read/fnvkrtypyjyk)

<!--
- Sep 11, 2022 : [Verilog and Multiplexers]({{site.baseurl}}/lab_pdfs/ECE275_Lab2_Multiplexers_Verilog_and_Schematics.pdf)
- Sep 18, 2022 : [Verilog Modules]({{site.baseurl}}/lab_pdfs/ECE275_Lab3_Verilog_Modules.pdf)
- Sep 25, 2022 : [Ripple Adder]({{site.baseurl}}/lab_pdfs/ECE275_Lab4.pdf)
- Oct 02, 2022 : [Logic Synthesis and Always Blocks]({{site.baseurl}}/lab_pdfs/ECE275_Lab5.pdf)
- Oct 23, 2022 : [Procedural Verilog and Binary to BCD]({{site.baseurl}}/lab_pdfs/ECE275_Lab6.pdf)
- Oct 30, 2022  : [Verilog Code Quality]({{site.baseurl}}/lab_pdfs/ECE275_Lab7.pdf)
- Nov 14, 2022 : [HDL Simulation]({{site.baseurl}}/lab_pdfs/ECE275_Lab8.pdf)
- Nov 17, 2022 : [Pong project Kickoff]({{site.baseurl}}/lab_pdfs/pongproject-kickoff/kickoff.pdf)
-->

# Lab supporting documents

- Pin Assignments : [DE0_TOP.qsf]({{site.baseurl}}/lab_pdfs/DE0_TOP.qsf)
- Top level file with all pins: [DE0_TOP.v]({{site.baseurl}}/lab_pdfs/DE0_TOP.v)
- DE0 FPGA Manual : [DEO User Manual](https://intel.com/content/dam/altera-www/global/en_US/portal/dsn/42/doc-us-dsnbk-42-5804152209-de0-user-manual.pdf)

# Prelabs

- Prelab for Lab 5 : [Lab 5 Prelab]({{site.baseurl}}/lab_pdfs/ECE275_Lab5_Prelab.pdf)

# Additional Resources

- Intel/Altera Document on Synthesis of Math Functions : [Advanced Synthesis Cookbook](https://intel.com/content/dam/www/programmable/us/en/pdfs/literature/manual/stx_cookbook.pdf)
- [Accompanying Cookbook code](https://github.com/thomasrussellmurphy/stx_cookbook)
-  [Parker Dillmann's DE0_Drivers](https://github.com/LonghornEngineer/DE0_Drivers)

# VGA Driver Files

- VGA Video Tutorial (Must be logged in to you Umaine account to view) : [Video Tutorial](https://drive.google.com/file/d/1KwSqLo8CvzKBAjxMmDpdbc_UMAonZH9S/view?usp=sharing)
	- There is a mistake in the video when instantiating the make box module, it should be make_box make_first_player_paddle( and not module make_first_player make_box(
- Example simple top level : [VGA_top.v]({{site.baseurl}}/lab_pdfs/final_project_vga_files/VGA_top.v)
- DE0 VGA Driver Module : [DE0_VGA.v]({{site.baseurl}}/lab_pdfs/final_project_vga_files/DE0_VGA.v)
- PLL (Phase Locked Loop) Verilog File : [PLL_PIXEL_CLK.v]({{site.baseurl}}/lab_pdfs/final_project_vga_files/PLL_PIXEL_CLK.v)
- QSF File : [VGA_top.qsf]({{site.baseurl}}/lab_pdfs/final_project_vga_files/VGA_top.qsf)


# Testbenches

- Tutorial video on testbenches (Must be logged in to you Umaine account to view) : [Testbenches Tutorial](https://drive.google.com/file/d/1_xbmeY4J0596vx63K-ubjDB998AcH6LX/view?usp=sharing)

# How to display text on VGA

- You will need the following four files
  + [display_message.v](../verilog/tic-tac-toe-dec-7/display_message.v)
  + [draw_ascii.v](../verilog/tic-tac-toe-dec-7/draw_ascii.v)
  + [rom_async.v](../verilog/tic-tac-toe-dec-7/rom_async.v)
  + [font_unscii_8x8_latin_uc.mem](../verilog/tic-tac-toe-dec-7/font_unscii_8x8_latin_uc.mem)
- Usage:

<pre><code class="language-verilog">
wire [11:0]  text_message_pixel_color;
wire [0:8*7-1] msg_7char = "RUNNING"; // only supports all casp
display_message #(
                     .MSG_LENGTH(7), // length of message
                     .char_width(32), // char width in pixels
                     .char_height(32), // char height in pixels
                     .INIT_F("font_unscii_8x8_latin_uc.mem") // font file
                     ) disp_msg
     (.X_pix(X_pix), // current X_pix being drawn
      .Y_pix(Y_pix), // current Y_pix being drawn
      .MSG(msg_7char), // Message as 8-bit ascii value
      .msg_x(RESX / 2 - 4*3*BALLWIDTH), // X pos of left top corner of msg
      .msg_y(RESY / 2 - 2*BALLWIDTH), // Y pos of left top corner of msg
      .char_color(12'b1100_0000_0000), // text color
      .text_bg_color(12'b1111_1111_1111), // text background color
      .prev_layer_color(BGCOLOR), // color from previous layers 
      .pixel_color(text_message_pixel_color)); // output wire for pixel color


always @(posedge pixel_clk)
	begin
      if (ballpixel)
        pixel_color <= BALLCOLOR;
      else if (paddlepixel)
        pixel_color <= PADDLECOLOR;
      else
        pixel_color <= text_message_pixel_color;
	end
</code></pre>
