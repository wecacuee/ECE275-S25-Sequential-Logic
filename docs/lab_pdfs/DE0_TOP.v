// Modified from 
// de0_debouncing_ctr.v: http://www.terasic.com.tw/cgi-bin/page/archive.pl?Language=English&No=364&PartNo=4
module DE0_TOP
 (
  //// Source: de0_bebounce_cnt.v Downloaded from: http://www.terasic.com.tw/cgi-bin/page/archive.pl?Language=English&No=364&PartNo=4
  //////////////////// Clock Input   ////////////////////  
  CLOCK_50,      // 50 MHz
  //////////////////// Push Button  ////////////////////
  ORG_BUTTON,      // Pushbutton[2:0]
  //////////////////// 7-SEG Dispaly ////////////////////
  HEX0_D,       // Seven Segment Digit 0
  HEX1_D,       // Seven Segment Digit 1
  HEX2_D,       // Seven Segment Digit 2
  HEX3_D,       // Seven Segment Digit 3
  //////////////////// VGA  ////////////////////////////
  VGA_HS,       // VGA H_SYNC
  VGA_VS,       // VGA V_SYNC
  VGA_R,         // VGA Red[3:0]
  VGA_G,        // VGA Green[3:0]
  VGA_B        // VGA Blue[3:0]
 );
//// Source: de0_bebounce_cnt.v Downloaded from: http://www.terasic.com.tw/cgi-bin/page/archive.pl?Language=English&No=364&PartNo=4
//////////////////////// Clock Input   ////////////////////////
input   CLOCK_50;    // 50 MHz
// input   CLOCK_50_2;    // 50 MHz
//////////////////////// Push Button  ////////////////////////
input [2:0] ORG_BUTTON;    // Pushbutton[2:0]
//////////////////////// 7-SEG Dispaly ////////////////////////
output [6:0] HEX0_D;     // Seven Segment Digit 0
output [6:0] HEX1_D;     // Seven Segment Digit 1
output [6:0] HEX2_D;     // Seven Segment Digit 2
output [6:0] HEX3_D;     // Seven Segment Digit 3
//////////////////////// VGA   ////////////////////////////
output   VGA_HS;     		// VGA H_SYNC
output   VGA_VS;     		// VGA V_SYNC
output [3:0] VGA_R;        // VGA Red[3:0]
output [3:0] VGA_G;      	// VGA Green[3:0]
output [3:0] VGA_B;       	// VGA Blue[3:0]

//==================================================================
//  REG/WIRE declarations
//==================================================================


wire    [2:0]  BUTTON;   // Button after debounce
wire       enter_current_cell;  // press enter


wire  [3:0]   input_cell_cursor;
wire  [8:0]   grid_state_marked;
wire  [8:0]   grid_state_x;
wire          player_x_turn; 
wire          someone_won; 
wire          player_x_won;
wire          errno;
wire          reset_button;
wire          next_valid_cell;

ticTacToe_debouncer debouncer(
 .CLOCK(CLOCK_50),
 .ORG_BUTTON(ORG_BUTTON),
 .BUTTON(BUTTON));

 

ticTacToeCore ttc(
 .CLOCK(CLOCK_50),
 .reset_n_in(1'b1),
 .BUTTON(BUTTON),
 .grid_state_marked(grid_state_marked),
 .grid_state_x(grid_state_x), 
 .reset_out(reset_button),
 .cell_cursor(input_cell_cursor),
 .cell_enter(enter_current_cell),
 .next_valid_cell(next_valid_cell),
 .player_x_turn(player_x_turn),
 .someone_won(someone_won),
 .player_x_won(player_x_won),
 .errno(errno)
);


ticTacToe_output_7seg tto_7seg(
 .CLOCK(CLOCK_50),
 .reset_n(reset_button),
 .next_valid_cell(next_valid_cell),
 .current_cell(input_cell_cursor),
 .enter_current_cell(enter_current_cell),
 .grid_state_marked(grid_state_marked),
 .player_x_turn(player_x_turn),
 .someone_won(someone_won),
 .player_x_won(player_x_won),
 .errno(errno),
 .SEG7_4({HEX3_D, HEX2_D, HEX1_D, HEX0_D})
);


ticTacToe_output_vga  #(
    .INIT_F("font_unscii_8x8_latin_uc.mem")
) tto_vga (
 .CLOCK(CLOCK_50),
 .reset_n(reset_button),
 .next_valid_cell(next_valid_cell),
 .current_cell(input_cell_cursor),
 .enter_current_cell(enter_current_cell),
 .grid_state_marked(grid_state_marked),
 .grid_state_x(grid_state_x),
 .player_x_turn(player_x_turn),
 .someone_won(someone_won),
 .player_x_won(player_x_won),
 .errno(errno),
 .VGA_HS(VGA_HS),     		// VGA H_SYNC
 .VGA_VS(VGA_VS),     		// VGA V_SYNC
 .VGA_R(VGA_R),        // VGA Red[3:0]
 .VGA_G(VGA_G),      	// VGA Green[3:0]
 .VGA_B(VGA_B)       	// VGA Blue[3:0]
);



endmodule