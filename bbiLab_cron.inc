<?php

function bbiLab_cron() {     
    BbiBuffer::check_buffer_entrada();
    BbiBuffer::send_buffer_salida();
    
}
