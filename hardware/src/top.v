module top
#(
parameter AD_WIDTH = 14
)
(
    //reference clock
    input                     sys_clk_p,
    input                     sys_clk_n,
    input                     rst_n,
    input                     trig_in, 
    //ad9518 signals
	output                    clk_spi_cs/* synthesis PAP_MARK_DEBUG="true" */,	
	output                    clk_spi_sclk/* synthesis PAP_MARK_DEBUG="true" */,
	inout                     clk_spi_sdio/* synthesis PAP_MARK_DEBUG="true" */,
// AD out
    output                    adc1_data_14_a,
    output                    adc1_clk,
//AD SPI
	output                    adc1_spi_cs/* synthesis PAP_MARK_DEBUG="true" */,
	output                    adc2_spi_cs/* synthesis PAP_MARK_DEBUG="true" */,
//AD1and AD2 share
	inout                     adc_spi_mosi,
    // AD1 signals
    input                     adc1_of_p,
    input                     adc1_of_n,
	input                     adc1_clk_p,
	input                     adc1_clk_n,	
	input[AD_WIDTH/2-1:0]     adc1_data_ap,	
	input[AD_WIDTH/2-1:0]     adc1_data_an,
	input[AD_WIDTH/2-1:0]     adc1_data_bp,	
	input[AD_WIDTH/2-1:0]     adc1_data_bn,
	// AD2 signals
    input                     adc2_of/* synthesis PAP_MARK_DEBUG="true" */,
	input                     adc2_clk_p,
	input                     adc2_clk_n,
	input[AD_WIDTH/2-1:0]     adc2_data_ap,	
	input[AD_WIDTH/2-1:0]     adc2_data_an,
	input[AD_WIDTH/2-1:0]     adc2_data_bp,	
	input[AD_WIDTH/2-1:0]     adc2_data_bn
    );
                                 
wire 							clk_50m;

wire[9:0]                       clk_lut_index/* synthesis PAP_MARK_DEBUG="true" */;
wire[23:0]                      clk_lut_data/* synthesis PAP_MARK_DEBUG="true" */;
wire[9:0]                       adc_lut_index/* synthesis PAP_MARK_DEBUG="true" */;
wire[23:0]                      adc_lut_data/* synthesis PAP_MARK_DEBUG="true" */;

wire                            adc1_clk/* synthesis PAP_MARK_DEBUG="true" */;
wire                            adc2_clk/* synthesis PAP_MARK_DEBUG="true" */;

wire[AD_WIDTH/2-1:0]            adc1_data_a/* synthesis PAP_MARK_DEBUG="true" */;
wire[AD_WIDTH/2-1:0]            adc1_data_a_delay;
wire[AD_WIDTH/2-1:0]            adc2_data_a_delay;
wire[AD_WIDTH/2-1:0]            adc2_data_b_delay;
wire[AD_WIDTH/2-1:0]            adc1_data_a_neg/* synthesis PAP_MARK_DEBUG="true" */;
wire[AD_WIDTH/2-1:0]            adc1_data_a_pos/* synthesis PAP_MARK_DEBUG="true" */;
wire[AD_WIDTH/2-1:0]            adc1_data_b/* synthesis PAP_MARK_DEBUG="true" */;
wire[AD_WIDTH/2-1:0]            adc1_data_b_delay;
wire[AD_WIDTH/2-1:0]            adc1_data_b_neg/* synthesis PAP_MARK_DEBUG="true" */;
wire[AD_WIDTH/2-1:0]            adc1_data_b_pos/* synthesis PAP_MARK_DEBUG="true" */;

wire[AD_WIDTH/2-1:0]            adc2_data_a/* synthesis PAP_MARK_DEBUG="true" */;
wire[AD_WIDTH/2-1:0]            adc2_data_a_neg/* synthesis PAP_MARK_DEBUG="true" */;
wire[AD_WIDTH/2-1:0]            adc2_data_a_pos/* synthesis PAP_MARK_DEBUG="true" */;
wire[AD_WIDTH/2-1:0]            adc2_data_b/* synthesis PAP_MARK_DEBUG="true" */;
wire[AD_WIDTH/2-1:0]            adc2_data_b_neg/* synthesis PAP_MARK_DEBUG="true" */;
wire[AD_WIDTH/2-1:0]            adc2_data_b_pos/* synthesis PAP_MARK_DEBUG="true" */;
(* MARK_DEBUG="true" *)wire adc1_data_a_of/* synthesis PAP_MARK_DEBUG="true" */;
(* MARK_DEBUG="true" *)wire adc1_data_b_of/* synthesis PAP_MARK_DEBUG="true" */;
(* MARK_DEBUG="true" *)wire adc2_data_a_of/* synthesis PAP_MARK_DEBUG="true" */;
(* MARK_DEBUG="true" *)wire adc2_data_b_of/* synthesis PAP_MARK_DEBUG="true" */;
`ifdef AD_WIDTH_12
wire[11:0] adc1_data_12_a/* synthesis PAP_MARK_DEBUG="true" */;
wire[11:0] adc1_data_12_b/* synthesis PAP_MARK_DEBUG="true" */;
wire[11:0] adc2_data_12_a/* synthesis PAP_MARK_DEBUG="true" */;
wire[11:0] adc2_data_12_b/* synthesis PAP_MARK_DEBUG="true" */;

reg[11:0] adc1_data_12_a_offset_d0/* synthesis PAP_MARK_DEBUG="true" */;
reg[11:0] adc1_data_12_b_offset_d0/* synthesis PAP_MARK_DEBUG="true" */;
reg[11:0] adc2_data_12_a_offset_d0/* synthesis PAP_MARK_DEBUG="true" */;
reg[11:0] adc2_data_12_b_offset_d0/* synthesis PAP_MARK_DEBUG="true" */;
`else
(* MARK_DEBUG="true" *)wire[13:0] adc1_data_14_a/* synthesis PAP_MARK_DEBUG="true" */;
(* MARK_DEBUG="true" *)wire[13:0] adc1_data_14_b/* synthesis PAP_MARK_DEBUG="true" */;
(* MARK_DEBUG="true" *)wire[13:0] adc2_data_14_a/* synthesis PAP_MARK_DEBUG="true" */;
(* MARK_DEBUG="true" *)wire[13:0] adc2_data_14_b/* synthesis PAP_MARK_DEBUG="true" */;
(* MARK_DEBUG="true" *)reg[13:0] adc1_data_14_a_offset_d0/* synthesis PAP_MARK_DEBUG="true" */;
(* MARK_DEBUG="true" *)reg[13:0] adc1_data_14_b_offset_d0/* synthesis PAP_MARK_DEBUG="true" */;
(* MARK_DEBUG="true" *)reg[13:0] adc2_data_14_a_offset_d0/* synthesis PAP_MARK_DEBUG="true" */;
(* MARK_DEBUG="true" *)reg[13:0] adc2_data_14_b_offset_d0/* synthesis PAP_MARK_DEBUG="true" */;

`endif 
 


wire               adc1_of/* synthesis PAP_MARK_DEBUG="true" */;
wire               adc1_of_delay;
wire               adc2_of_delay;
wire			   ad9518_pll_locked /* synthesis PAP_MARK_DEBUG="true" */;
wire               adc_spi_cs/* synthesis PAP_MARK_DEBUG="true" */;
wire               adc_spi_dir  ;
wire			   adc_spi_in   ;
wire			   adc_spi_out  ;


wire               clk_spi_dir  /* synthesis PAP_MARK_DEBUG="true" */;
wire			   clk_spi_in   /* synthesis PAP_MARK_DEBUG="true" */;
wire			   clk_spi_out  /* synthesis PAP_MARK_DEBUG="true" */;

wire               free_clk_g  ;                  
wire               ref_clk     ;
wire               sys_rst_n;
wire delay_rst_n;
wire clk_200m;
power_on_rst #
  (
	.CLK_FRE (50    ),
	.DELAY_MS(20    )
  ) 
reset_power_on_m0
(            
	.clk                        (clk_50m                  ),               
	.rst_n                      (rst_n                    ),               //user reset high active
	.power_on_rstn              (delay_rst_n              )              //power on reset low active     
);

wire clk_spi_clk;
wire clk_spi_adc;
assign  clk_spi_sclk  = ad9518_pll_locked ? clk_spi_adc : clk_spi_clk;

assign adc_spi_mosi = ~adc_spi_dir ? adc_spi_out : 1'bz;
assign adc_spi_in = adc_spi_mosi;

assign adc2_spi_cs = adc_spi_cs;
assign adc1_spi_cs = adc_spi_cs;

assign clk_spi_sdio = ~clk_spi_dir ? clk_spi_out : 1'bz;
assign clk_spi_in = clk_spi_sdio;



wire adc1_clk_ibuf;
wire adc1_clk_w;
wire adc2_clk_ibuf;
wire adc2_clk_w;
wire sys_clk;


sys_pll sys_pll_m0
 (
	// Clock in ports
	.clk_in1_p              (sys_clk_p                   ),
	.clk_in1_n              (sys_clk_n                   ),
	// Clock out ports
	.clk_out1               (clk_50m                   ),
	.clk_out2               (clk_200m                  ),
	// Status and control signals
    .reset(1'b0), // input reset
	.locked                 (                  )
 );


wire button_posedge;
wire button_negedge;
ax_debounce ax_debounce_m0
(
    .clk             (clk_50m       ),
    .rst             (~rst_n  ),
    .button_in       (trig_in       ),
    .button_posedge  (button_posedge),
    .button_negedge  (),
    .button_out      (              )
);
localparam IDLE = 0;
localparam TRIG = 1;
reg [1:0]state/* synthesis PAP_MARK_DEBUG="true" */;
reg trig_en/* synthesis PAP_MARK_DEBUG="true" */;

always@(posedge clk_50m or negedge rst_n)
begin
        if(~rst_n)begin
            trig_en <= 1'b0;
            state <= IDLE;end
        else begin

    		case(state)
    			IDLE:
    			begin
                   if(button_posedge)
    				  state <= TRIG;
                    else 
                      state <= IDLE;
    			end
    			TRIG:begin
                   state <= TRIG;
                   trig_en <= 1'b1;
                end
    			default:
    				state <= IDLE;
    		endcase
       end
end


//ad9518 ʱ��
spi_config spi_config_clk(
	.rst                        (~delay_rst_n            ),
	.clk                        (clk_50m                 ),
	.clk_div_cnt                (16'd500                 ),
    .spi_addr_2byte             (1'b1),
	.lut_index                  (clk_lut_index           ),
	.lut_reg_addr               (clk_lut_data[23:8]      ),
	.lut_reg_data               (clk_lut_data[7:0]       ),
	.pll_check                  (1'b1                    ),
	.pll_locked                 (ad9518_pll_locked       ),
//	.error                      (error2                  ),
//	.done                       (done2                   ),	
	.spi_ce                     (clk_spi_cs              ),
	.spi_sclk                   (clk_spi_clk             ),
	.spi_dir          			(clk_spi_dir             ),
	.spi_in          			(clk_spi_in              ),
	.spi_out         			(clk_spi_out             )
);  
ad9518_lut_config lut_config_clk(
	.lut_index                  (clk_lut_index           ),
	.lut_data                   (clk_lut_data            )
);

spi_8bit_config spi_config_adc(
	.rst                        (~ad9518_pll_locked                  ),
	.clk                        (clk_50m                  ),
	.clk_div_cnt                (16'd500                  ),
    .spi_addr_2byte             (1'b0),  // input
	.lut_index                  (adc_lut_index           ),
	.lut_reg_addr               (adc_lut_data[23:8]      ),
	.lut_reg_data               (adc_lut_data[7:0]       ),
	.pll_check                  ( 1'b0                   ),
//	.error                      (error1                         ),
//	.done                       (done1                         ),	
	.spi_ce                     (adc_spi_cs               ),
	.spi_sclk                   (clk_spi_adc                         ),
	.spi_dir                    (adc_spi_dir              ),
	.spi_in          			(adc_spi_in               ),
	.spi_out         			(adc_spi_out              )
);
//configure look-up table
ad2512_lut_config lut_config_adc(
	.lut_index                  (adc_lut_index           ),
	.lut_data                   (adc_lut_data            )
);


wire adc1_clk_s;
wire adc2_clk_s;

   IBUFDS INBUFDS_adc1_clk (
      .O(adc1_clk_ibuf),   // 1-bit output: Buffer output
      .I(adc1_clk_p),   // 1-bit input: Diff_p buffer input (connect directly to top-level port)
      .IB(adc1_clk_n)  // 1-bit input: Diff_n buffer input (connect directly to top-level port)
   );
     adc1_shift adc1_shift_u0
   (
    // Clock out ports
    .clk_out1(adc1_clk),     // output clk_out1
    // Status and control signals
    .reset(1'b0), // input reset
    .locked(),       // output locked
   // Clock in ports
    .clk_in1(adc1_clk_ibuf));      // input clk_in1


   IBUFDS INBUFDS_adc1_of (
      .O(adc1_of),   // 1-bit output: Buffer output
      .I(adc1_of_p),   // 1-bit input: Diff_p buffer input (connect directly to top-level port)
      .IB(adc1_of_n)  // 1-bit input: Diff_n buffer input (connect directly to top-level port)
   );

    IDDR #(
          .DDR_CLK_EDGE("SAME_EDGE_PIPELINED"),
          .INIT_Q1(1'b0),
          .INIT_Q2(1'b0),
          .SRTYPE("SYNC") // Set/Reset type: "SYNC" or "ASYNC" 
       ) adc_of_inst0 (
          .Q1(adc1_data_a_of), // 1-bit output for positive edge of clock
          .Q2(adc1_data_b_of), // 1-bit output for negative edge of clock
          .C(adc1_clk),   // 1-bit clock input
          .CE(1'b1), // 1-bit clock enable input
          .D(adc1_of),   // 1-bit DDR data input
          .R(1'b0),   // 1-bit reset
          .S(1'b0)    // 1-bit set
       );
		
genvar i;
generate
	for (i = 0; i < AD_WIDTH/2; i = i + 1) begin:IBUFDS_DATAS


		IBUFDS IBUFDS_adc1_data_a (
		.O   	(adc1_data_a[i]),  // Buffer output
		.I   	(adc1_data_ap[i]),  // Diff_p buffer input (connect directly to top-level port)
		.IB  	(adc1_data_an[i]) // Diff_n buffer input (connect directly to top-level port)
		);

      IDDR #(
          .DDR_CLK_EDGE("SAME_EDGE_PIPELINED"),
          .INIT_Q1(1'b0),
          .INIT_Q2(1'b0),
          .SRTYPE("SYNC") // Set/Reset type: "SYNC" or "ASYNC" 
       ) IDDR_adc1_data_a (
          .Q1(adc1_data_a_neg[i]), // 1-bit output for positive edge of clock
          .Q2(adc1_data_a_pos[i]), // 1-bit output for negative edge of clock
          .C(adc1_clk),   // 1-bit clock input
          .CE(1'b1), // 1-bit clock enable input
          .D(adc1_data_a[i]),   // 1-bit DDR data input
          .R(1'b0),   // 1-bit reset
          .S(1'b0)    // 1-bit set
       );

        
				IBUFDS IBUFDS_adc1_data_b (
		.O   	(adc1_data_b[i]),  // Buffer output
		.I   	(adc1_data_bp[i]),  // Diff_p buffer input (connect directly to top-level port)
		.IB  	(adc1_data_bn[i]) // Diff_n buffer input (connect directly to top-level port)
		);

      IDDR #(
          .DDR_CLK_EDGE("SAME_EDGE_PIPELINED"), // "OPPOSITE_EDGE", "SAME_EDGE"  or "SAME_EDGE_PIPELINED" 
          .INIT_Q1(1'b0), // Initial value of Q1: 1'b0 or 1'b1
          .INIT_Q2(1'b0), // Initial value of Q2: 1'b0 or 1'b1
          .SRTYPE("SYNC") // Set/Reset type: "SYNC" or "ASYNC" 
       ) IDDR_adc1_data_b (
          .Q1(adc1_data_b_neg[i]), // 1-bit output for positive edge of clock
          .Q2(adc1_data_b_pos[i]), // 1-bit output for negative edge of clock
          .C(adc1_clk),   // 1-bit clock input
          .CE(1'b1), // 1-bit clock enable input
          .D(adc1_data_b[i]),   // 1-bit DDR data input
          .R(1'b0),   // 1-bit reset
          .S(1'b0)    // 1-bit set
       );
	end
endgenerate


assign adc1_data_14_a ={
                            adc1_data_a_pos[6],adc1_data_a_neg[6],
                            adc1_data_a_pos[5],adc1_data_a_neg[5],
                            adc1_data_a_pos[4],adc1_data_a_neg[4],
                            adc1_data_a_pos[3],adc1_data_a_neg[3],
                            adc1_data_a_pos[2],adc1_data_a_neg[2],
                            adc1_data_a_pos[1],adc1_data_a_neg[1],
                            adc1_data_a_pos[0],adc1_data_a_neg[0]
                            };

endmodule