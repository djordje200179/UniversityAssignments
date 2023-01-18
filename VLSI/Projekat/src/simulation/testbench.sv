`include "uvm_macros.svh"

import uvm_pkg::*;

interface ps2_if(input bit clk);
    logic rst;

    logic data_pin;
    logic clk_pin;

    logic data_req;
    logic [7:0] data;
    logic data_err;
    logic data_ack;
endinterface

class ps2_item extends uvm_sequence_item;
    rand bit [10:0] data_in;

    bit [7:0] data_out;
    bit data_err;

    constraint start_bit { data_in[0] == 0; }
    constraint end_bit { data_in[10] == 1; }

    `uvm_object_utils_begin(ps2_item)
        `uvm_field_int(data_in, UVM_DEFAULT)
        `uvm_field_int(data_out, UVM_NOPRINT)
    `uvm_object_utils_end
    
    function new(string name = "ps2_item");
        super.new(name);
    endfunction
    
    virtual function string my_print();
        return $sformatf(
            "data_in = %10b data_out = %2h data_err = %1b", 
            data_in, data_out, data_err
        );
    endfunction
endclass

class generator extends uvm_sequence;
    `uvm_object_utils(generator)
    
    function new(string name = "generator");
        super.new(name);
    endfunction
    
    localparam ITERATIONS = 20;
    
    virtual task body();
        for (int i = 0; i < ITERATIONS; i++) begin
            ps2_item item = ps2_item::type_id::create("item");
            start_item(item);
            item.randomize();
            
            `uvm_info("Generator", $sformatf("Item %0d/%0d created", i + 1, ITERATIONS), UVM_LOW)
            //item.print();
            
            finish_item(item);
        end
    endtask
endclass

class driver extends uvm_driver #(ps2_item);
    `uvm_component_utils(driver)
    
    function new(string name = "driver", uvm_component parent = null);
        super.new(name, parent);
    endfunction
    
    virtual ps2_if vif;
    
    virtual function void build_phase(uvm_phase phase);
        super.build_phase(phase);
        
        if (!uvm_config_db#(virtual ps2_if)::get(this, "", "ps2_vif", vif)) 
            `uvm_fatal("Driver", "No interface.")
    endfunction
    
    virtual task run_phase(uvm_phase phase);
        super.run_phase(phase);

        vif.clk_pin <= 1;
        vif.data_pin <= 1;
        
        forever begin
            ps2_item item;
            seq_item_port.get_next_item(item);

            `uvm_info("Driver", item.my_print(), UVM_LOW)
            
            for (int i = 0; i < 11; i++) begin
                vif.data_pin <= item.data_in[i];

                vif.clk_pin <= 0;
                #5000;
                vif.clk_pin <= 1;
                #5000;
            end
            
            vif.data_pin <= 1;

            seq_item_port.item_done();
        end
    endtask
endclass

class monitor extends uvm_monitor;
    `uvm_component_utils(monitor)
    
    function new(string name = "monitor", uvm_component parent = null);
        super.new(name, parent);
    endfunction
    
    virtual ps2_if vif; 
    uvm_analysis_port #(ps2_item) mon_analysis_port;
    
    virtual function void build_phase(uvm_phase phase);
        super.build_phase(phase);
        
        if (!uvm_config_db#(virtual ps2_if)::get(this, "", "ps2_vif", vif)) 
            `uvm_fatal("Monitor", "No interface.")
        
        mon_analysis_port = new("mon_analysis_port", this);
    endfunction
    
    virtual task run_phase(uvm_phase phase);
        super.run_phase(phase);
        
        forever begin
            ps2_item item = ps2_item::type_id::create("item");
            
            vif.data_req = 0;
            @(posedge vif.clk); 

            repeat (11) begin
                @(negedge vif.clk_pin);
                item.data_in = {vif.data_pin, item.data_in[10:1]};      
            end

            vif.data_req = 1;
            @(posedge vif.data_ack);
            item.data_out = vif.data;
            item.data_err = vif.data_err;
            
            @(posedge vif.clk);
            vif.data_req = 1;
            @(posedge vif.clk);
            
            `uvm_info("Monitor", item.my_print(), UVM_LOW)
            mon_analysis_port.write(item);
        end
    endtask
endclass

class agent extends uvm_agent;
    `uvm_component_utils(agent)
    
    function new(string name = "agent", uvm_component parent = null);
        super.new(name, parent);
    endfunction
    
    driver d0;
    monitor m0;
    
    uvm_sequencer #(ps2_item) s0;
    
    virtual function void build_phase(uvm_phase phase);
        super.build_phase(phase);

        d0 = driver::type_id::create("d0", this);
        m0 = monitor::type_id::create("m0", this);
        s0 = uvm_sequencer#(ps2_item)::type_id::create("s0", this);
    endfunction
    
    virtual function void connect_phase(uvm_phase phase);
        super.connect_phase(phase);
        
        d0.seq_item_port.connect(s0.seq_item_export);
    endfunction
endclass

class scoreboard extends uvm_scoreboard;
    `uvm_component_utils(scoreboard)
    
    function new(string name = "scoreboard", uvm_component parent = null);
        super.new(name, parent);
    endfunction
    
    uvm_analysis_imp #(ps2_item, scoreboard) mon_analysis_imp;
    
    virtual function void build_phase(uvm_phase phase);
        super.build_phase(phase);
        
        mon_analysis_imp = new("mon_analysis_imp", this);
    endfunction
    
    bit [7:0] data = 8'h00;
    bit data_err = 1'b0;
    
    virtual function write(ps2_item item);
        data = item.data_in[8:1];
        data_err = ~^item.data_in[8:1] == item.data_in[9];

        if (data == item.data_out && data_err == item.data_err)
            `uvm_info("Scoreboard", "PASS!", UVM_LOW)
        else
            `uvm_error("Scoreboard", 
                $sformatf(
                    "FAIL! expected: data = %2h, data_err = %1b; got: data = %2h, data_err = %1b",
                    data, data_err, item.data_out, item.data_err
                )
            );
    endfunction
endclass

class env extends uvm_env;
    `uvm_component_utils(env)
    
    function new(string name = "env", uvm_component parent = null);
        super.new(name, parent);
    endfunction
    
    agent a0;
    scoreboard sb0;
    
    virtual function void build_phase(uvm_phase phase);
        super.build_phase(phase);
        
        a0 = agent::type_id::create("a0", this);
        sb0 = scoreboard::type_id::create("sb0", this);
    endfunction
    
    virtual function void connect_phase(uvm_phase phase);
        super.connect_phase(phase);
        a0.m0.mon_analysis_port.connect(sb0.mon_analysis_imp);
    endfunction
endclass

class test extends uvm_test;
    `uvm_component_utils(test)
    
    function new(string name = "test", uvm_component parent = null);
        super.new(name, parent);
    endfunction
    
    virtual ps2_if vif;
    
    env e0;
    generator g0;
    
    virtual function void build_phase(uvm_phase phase);
        super.build_phase(phase);
        
        if (!uvm_config_db#(virtual ps2_if)::get(this, "", "ps2_vif", vif))
            `uvm_fatal("Test", "No interface.")

        e0 = env::type_id::create("e0", this);
        g0 = generator::type_id::create("g0");
    endfunction

    virtual function void end_of_elaboration_phase(uvm_phase phase);
        uvm_top.print_topology();
    endfunction

    virtual task run_phase(uvm_phase phase);
        phase.raise_objection(this);
        
        vif.rst <= 1;
        #20 vif.rst <= 0;
        
        g0.start(e0.a0.s0);
        phase.drop_objection(this);
    endtask
endclass

module testbench;
    reg clk;

    ps2_if dut_if (
        .clk(clk)
    );
        
    ps2 dut (
        .clk(clk),
        .rst(dut_if.rst),
        .data_pin(dut_if.data_pin),
        .clk_pin(dut_if.clk_pin),
        .data_req(dut_if.data_req),
        .data(dut_if.data),
        .data_err(dut_if.data_err),
        .data_ack(dut_if.data_ack)
    );

    initial begin
        clk = 0;
        forever #5 clk = ~clk;
    end

    initial begin
        uvm_config_db#(virtual ps2_if)::set(
            null, "*", "ps2_vif", dut_if
        );
        
        run_test("test");
    end
endmodule
