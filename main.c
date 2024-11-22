#include <stdio.h>
#include "driver/i2c_master.h"
#include "freertos/FreeRTOS.h"

#define RAD_PTN 32

void ssd1306_init(i2c_master_dev_handle_t dev_handle);
void ssd1306_clear();
void ssd1306_dot(i2c_master_dev_handle_t dev_handle, uint8_t i, uint8_t j);
void ssd1306_output(i2c_master_dev_handle_t dev_handle);
void circle(int rad, int i_cent, int j_cent);


uint8_t cambus[64][128];

void app_main(void)
{
    i2c_master_bus_config_t i2c_mst_config = {
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .i2c_port = 0,
        .scl_io_num = 26,
        .sda_io_num = 25,
        .glitch_ignore_cnt = 7,
        .flags.enable_internal_pullup = true,
    };

    i2c_master_bus_handle_t bus_handle;
    i2c_new_master_bus(&i2c_mst_config, &bus_handle);

    i2c_device_config_t dev_cfg = {
        .dev_addr_length = I2C_ADDR_BIT_LEN_7,
        .device_address = 0x3C,
        .scl_speed_hz = 1000000,
    };

    i2c_master_dev_handle_t dev_handle;
    ESP_ERROR_CHECK(i2c_master_bus_add_device(bus_handle, &dev_cfg, &dev_handle));

    
    const uint8_t ctrl_cmd = 0x00;
    const uint8_t ctrl_data = 0x40;

    uint8_t buf_cmd[32];

/*    buf_cmd[0] = ctrl_cmd;
    buf_cmd[1] = 0xaf; //on
    buf_cmd[2] = 0xa5; //entire display on
    i2c_master_transmit(dev_handle,buf_cmd, 3, -1);

    vTaskDelay(pdMS_TO_TICKS(500));
    buf_cmd[0] = ctrl_cmd;
    buf_cmd[1] = 0xae; //off
    i2c_master_transmit(dev_handle,buf_cmd, 2, -1);

    vTaskDelay(pdMS_TO_TICKS(500));
    buf_cmd[0] = ctrl_cmd;
    buf_cmd[1] = 0xaf;
    i2c_master_transmit(dev_handle,buf_cmd, 2, -1);

    vTaskDelay(pdMS_TO_TICKS(500));
    buf_cmd[0] = ctrl_cmd;
    buf_cmd[1] = 0xae;
    buf_cmd[2] = 0x8d;
    buf_cmd[3] = 0x14;    
    i2c_master_transmit(dev_handle,buf_cmd, 4, -1);

    vTaskDelay(pdMS_TO_TICKS(500));
    buf_cmd[0] = ctrl_cmd;
    buf_cmd[1] = 0xaf;
    i2c_master_transmit(dev_handle,buf_cmd, 2, -1);*/

    uint8_t disp_buff1[132];
    disp_buff1[0] = ctrl_data;
    int i;
    for (i=1; i <= 131; i++){
        disp_buff1[i] = 0xff;
    }

    uint8_t disp_buff2[132];
    disp_buff2[0] = ctrl_data;
    for (i=1; i <= 131; i++){
        disp_buff2[i] = 0x00;
    }

    uint8_t disp_buff3[132];
    disp_buff3[0] = ctrl_data;
    for (i=1; i <= 131; i++){
        if(i%5==0){
            disp_buff3[i] = 0xff;
        }else{
            disp_buff3[i] = 0x00;
        }
    }

    ssd1306_init(dev_handle);

    vTaskDelay(pdMS_TO_TICKS(100));
    buf_cmd[0] = ctrl_cmd;
    buf_cmd[1] = 0xb0; //page0
    i2c_master_transmit(dev_handle,buf_cmd, 2, -1);
    buf_cmd[0] = ctrl_cmd;
    buf_cmd[1] = 0x00; 
    buf_cmd[2] = 0x10;  //start seg 0
    i2c_master_transmit(dev_handle,buf_cmd, 3, -1);
    i2c_master_transmit(dev_handle,disp_buff1, 129, -1);

    buf_cmd[0] = ctrl_cmd;
    buf_cmd[1] = 0xb1; //page1
    i2c_master_transmit(dev_handle,buf_cmd, 2, -1);
    buf_cmd[0] = ctrl_cmd;
    buf_cmd[1] = 0x00; 
    buf_cmd[2] = 0x10;  //start seg 0
    i2c_master_transmit(dev_handle,buf_cmd, 3, -1);
    i2c_master_transmit(dev_handle,disp_buff2, 129, -1);

    buf_cmd[0] = ctrl_cmd;
    buf_cmd[1] = 0xb2; //page2
    i2c_master_transmit(dev_handle,buf_cmd, 2, -1);
    buf_cmd[0] = ctrl_cmd;
    buf_cmd[1] = 0x00; 
    buf_cmd[2] = 0x10;  //start seg 0
    i2c_master_transmit(dev_handle,buf_cmd, 3, -1);
    i2c_master_transmit(dev_handle,disp_buff1, 129, -1);

    buf_cmd[0] = ctrl_cmd;
    buf_cmd[1] = 0xb3; //page3
    i2c_master_transmit(dev_handle,buf_cmd, 2, -1);
    buf_cmd[0] = ctrl_cmd;
    buf_cmd[1] = 0x00; 
    buf_cmd[2] = 0x10;  //start seg 0
    i2c_master_transmit(dev_handle,buf_cmd, 3, -1);
    i2c_master_transmit(dev_handle,disp_buff3, 129, -1);

    buf_cmd[0] = ctrl_cmd;
    buf_cmd[1] = 0xb4; //page4
    i2c_master_transmit(dev_handle,buf_cmd, 2, -1);
    buf_cmd[0] = ctrl_cmd;
    buf_cmd[1] = 0x00; 
    buf_cmd[2] = 0x10;  //start seg 0
    i2c_master_transmit(dev_handle,buf_cmd, 3, -1);
    i2c_master_transmit(dev_handle,disp_buff3, 129, -1);

    buf_cmd[0] = ctrl_cmd;
    buf_cmd[1] = 0xb5; //page5
    i2c_master_transmit(dev_handle,buf_cmd, 2, -1);
    buf_cmd[0] = ctrl_cmd;
    buf_cmd[1] = 0x00; 
    buf_cmd[2] = 0x10;  //start seg 0
    i2c_master_transmit(dev_handle,buf_cmd, 3, -1);
    i2c_master_transmit(dev_handle,disp_buff1, 129, -1);

    buf_cmd[0] = ctrl_cmd;
    buf_cmd[1] = 0xb6; //page6
    i2c_master_transmit(dev_handle,buf_cmd, 2, -1);
    buf_cmd[0] = ctrl_cmd;
    buf_cmd[1] = 0x00; 
    buf_cmd[2] = 0x10;  //start seg 0
    i2c_master_transmit(dev_handle,buf_cmd, 3, -1);
    i2c_master_transmit(dev_handle,disp_buff2, 129, -1);

    buf_cmd[0] = ctrl_cmd;
    buf_cmd[1] = 0xb7; //page7
    i2c_master_transmit(dev_handle,buf_cmd, 2, -1);
    buf_cmd[0] = ctrl_cmd;
    buf_cmd[1] = 0x00; 
    buf_cmd[2] = 0x10;  //start seg 0
    i2c_master_transmit(dev_handle,buf_cmd, 3, -1);
    i2c_master_transmit(dev_handle,disp_buff1, 129, -1);
    vTaskDelay(pdMS_TO_TICKS(500));

    ssd1306_clear();
    ssd1306_output(dev_handle);

//    int i;
    for(i=0; i<=63; i++){
        ssd1306_dot(dev_handle, i, i);
    }

    vTaskDelay(pdMS_TO_TICKS(500));

    ssd1306_clear();

    int j;
    for (i=0; i<=63; i++){
        for(j=0; j <=127; j++){
            cambus[i][j] = 0;
        }
        cambus[i][127-i] = 1;
    }
    ssd1306_output(dev_handle);
    printf("line done\n");
    vTaskDelay(pdMS_TO_TICKS(500));

    for (i=0; i<=63; i++){
        for(j=0; j <=63; j++){
            cambus[i][j] = 1;
        }
        for(j=64; j <=127; j++){
            cambus[i][j] = 0;
        }
    }
    ssd1306_output(dev_handle);
    vTaskDelay(pdMS_TO_TICKS(500));

    for (i=0; i<=63; i++){
        for(j=0; j <=127; j++){
            cambus[i][j] = 0;
        }
    }
    ssd1306_output(dev_handle);
    vTaskDelay(pdMS_TO_TICKS(500));
    circle(5, 31, 63);
    ssd1306_output(dev_handle);
    vTaskDelay(pdMS_TO_TICKS(500));
    circle(10, 31, 63);
    ssd1306_output(dev_handle);
    vTaskDelay(pdMS_TO_TICKS(500));
    circle(20, 31, 63);
    ssd1306_output(dev_handle);
    vTaskDelay(pdMS_TO_TICKS(500));
    circle(40, 31, 63);
    ssd1306_output(dev_handle);
    vTaskDelay(pdMS_TO_TICKS(500));

    ssd1306_clear();
    int r, x, y, vx, vy;
    r = 10;
    x = r;
    y = r;
    vx=2;
    vy=4;
    while(true){
        ssd1306_clear();
        circle(r, y, x);
        ssd1306_output(dev_handle);
        if((x<=r && vx < 0) || ( 127-r<=x && vx>0)){
            vx *= -1;
        }
        if((y<=r && vy < 0) || ( 63-r<=y && vy>0)){
            vy *= -1;
        }
        x +=vx;
        y +=vy;
        vTaskDelay(pdMS_TO_TICKS(50));
    }
}


void ssd1306_init(i2c_master_dev_handle_t dev_handle){
    const uint8_t init_ctrl_cmd = 0x00;
    const uint8_t init_ctrl_data = 0x40;

    uint8_t buf_cmd[32];

    buf_cmd[0] = init_ctrl_cmd;
    buf_cmd[1] = 0xae;  //off
    buf_cmd[2] = 0xa6;  //normal display
    buf_cmd[3] = 0x2e;  //deactivate scroll
    buf_cmd[4] = 0x20;  //set memory addressing mode
    buf_cmd[5] = 0x02;  //memory addressing mode = page addressing mode
    buf_cmd[6] = 0x00;  buf_cmd[7] = 0x10;  //column start address 0
    buf_cmd[8] = 0x40;  //display RAM stat line 0
    buf_cmd[9] = 0xa0;  //column address 0 is mapped to SEG0
    buf_cmd[10] = 0xa8; buf_cmd[11] = 0x3f; //multiplex ratio 63+1=64
    buf_cmd[12] = 0xa4; //GDDRAM
    buf_cmd[13] = 0x8d; buf_cmd[14] = 0x14; //charge pump enable 
    buf_cmd[15] = 0xaf;  //on
    i2c_master_transmit(dev_handle,buf_cmd, 16, -1);
    vTaskDelay(pdMS_TO_TICKS(100));
}

void ssd1306_clear(){
    int i, j;
    for (i=0; i<=63; i++){
        for(j=0; j <=127; j++){
            cambus[i][j] = 0;
        }
    }
}

void ssd1306_dot(i2c_master_dev_handle_t dev_handle, uint8_t i, uint8_t j){
    const uint8_t dot_ctrl_cmd = 0x00;
    const uint8_t dot_ctrl_data = 0x40;
    const uint8_t dot_cmd_page = 0xb0;

    uint8_t page = i/8;
    uint8_t com = i%8;

    uint8_t dot_buf_cmd[4];
    dot_buf_cmd[0]=dot_ctrl_cmd;
    dot_buf_cmd[1]=0xb0+page;
    dot_buf_cmd[2]=0x00 + (j & 0x0f); 
    dot_buf_cmd[3]=0x10 + (j >> 4); 
    i2c_master_transmit(dev_handle,dot_buf_cmd, 4, -1);

    uint8_t dot_buf_disp[2];
    dot_buf_disp[0] = dot_ctrl_data;
    dot_buf_disp[1] = (uint8_t)(1)<<(com);
    i2c_master_transmit(dev_handle,dot_buf_disp, 2, -1);    
}

void ssd1306_output(i2c_master_dev_handle_t dev_handle){
    uint8_t buf_out[8][129];
    uint8_t buf_cmd[3], ret_cmd[3];
    int i_cam, j_cam;
    int page, i_page, seg;

    for (page=0; page <= 7; page++){
        buf_out[page][0]=0x40; //graphic data tag
        for(seg=1; seg<=128; seg++){
            buf_out[page][seg]=0;
        }
    }
    for (i_cam=0; i_cam <= 63; i_cam++){
        page = i_cam/8;
        i_page = i_cam % 8;
        for (j_cam=0; j_cam <= 127; j_cam++){
            seg = j_cam+1;
            buf_out[page][seg] |= (cambus[i_cam][j_cam]) << i_page; 
        }
    }
    

    ret_cmd[0] = 0x00;//cmd tag
    ret_cmd[1] = 0x00;//start segment nibble
    ret_cmd[2] = 0x10;//start segment nibble

    buf_cmd[0] = 0x00; //cmd tag

    
    for (page=0; page <=7; page++){
        i2c_master_transmit(dev_handle, ret_cmd, 3, -1);
        buf_cmd[1] = 0xb0 | ((uint8_t)(page));
        i2c_master_transmit(dev_handle, buf_cmd, 2, -1);
        i2c_master_transmit(dev_handle, buf_out[page], 129, -1);
    }
}


void circle(int rad, int i_cent, int j_cent){
    const int rad_ptn = RAD_PTN;
    const int i_cent_ptn = RAD_PTN;
    const int j_cent_ptn = RAD_PTN;
    const uint8_t pattern[RAD_PTN*2+1][RAD_PTN*2+1] = {
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0},
        {0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0},
        {0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0},
        {0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0},
        {0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0},
        {0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0},
        {0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0},
        {0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0},
        {0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0},
        {0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0},
        {0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0},
        {0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0},
        {0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0},
        {0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0},
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
        {0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0},
        {0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0},
        {0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0},
        {0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0},
        {0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0},
        {0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0},
        {0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0},
        {0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0},
        {0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0},
        {0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0},
        {0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0},
        {0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0},
        {0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0},
        {0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
    };

    printf("array ready");

    int left, right, top, bottom;
    //描画範囲の決定
    if ((left=j_cent-rad)<0){
        left = 0;
    }
    if((right=j_cent+rad)>127){
        right=127;
    }
    if((top=i_cent-rad)<0){
        top = 0;
    }
    if((bottom=i_cent+rad)>63){
        bottom = 63;
    }

    int i_cam_abs, j_cam_abs;
    int i_cam_rel, j_cam_rel;
    int i_ptn_abs, j_ptn_abs;
    int i_ptn_rel, j_ptn_rel;
    const float scale = (float)(rad_ptn)/(float)(rad);

    for(i_cam_abs=top; i_cam_abs<=bottom; i_cam_abs++){
        i_cam_rel = i_cam_abs - i_cent;
        i_ptn_rel = (int)(scale*i_cam_rel);
        i_ptn_abs = i_ptn_rel + i_cent_ptn;
        for(j_cam_abs=left; j_cam_abs<=right; j_cam_abs++){
            j_cam_rel = j_cam_abs - j_cent;
            j_ptn_rel = (int)(scale*j_cam_rel);
            j_ptn_abs = j_ptn_rel + j_cent_ptn;
            cambus[i_cam_abs][j_cam_abs] = pattern[i_ptn_abs][j_ptn_abs];
        }
    }

}
