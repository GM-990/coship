#!/usr/bin/python
# ------------------------------------------------------------------------------------
# FileName:  memory_cfg.py
# Copyright (c) 2008 - 2010 Shen Zhen Coship Electronics CO., LTD.
# 
# Version:       Ver 1.0
# Author:        wuzhengbing (wuzhengbing@coship.com)
#
# File Description:
#        st71xx platform memory config script
# 
# Modify History:
# Version     Date        Author          Description
# --------    ----------  --------        ------------
# 1.0         2008-12-16  wuzhengbing     Create
# 1.1         2008-12-24  wuzhengbing     increase AVMEM_SYS and driver partition size
# 1.2         2008-12-30  wuzhengbing     for mmp linux environment
# 1.3         2008-01-04  wangte          double video decoder avmem size if PIP mode
# 1.4         2009-01-04  wuzhengbing     memory in LMI_VID can be cached,correct some spell error
# 1.5         2009-01-21  wuzhengbing     reduced AVMEM size for SD video decoding,
#                                         increase os_partition size & PVR support
# 1.6         2009-06-12  wuzhengbing     increase avmem_sys for HD-DEI
#                                         increase driver_partition&ncache_partition for PVR
# 1.7         2009-06-17  wuzhengbing     set OFFSET_FOR_AV_FW to 5M for st7105
#                                         subtract audio used avmem when PIP mode
# 1.8         2010-03-22  wuzhengbing     add compatibility for ST5197
# 1.9         2010-07-22  wuzhengbing     st40R4.4.0 for st7105
# 2.0	      2010-07-27  hanshaoyang     add compatibility for ST5289
# ------------------------------------------------------------------------------------
# DO NOT modify this file if you don't know the result. 
# ------------------------------------------------------------------------------------

import ConfigParser, os, sys, re, logging

#const
M = 1024*1024
K = 1024

VERSION = '2.0'
DONOT_MODIFY_NOTICE = 'automatically created by memory_cfg.py, do not modify manually!'

class Partition:
    base = 0
    size = 0
    name = ''
    def __init__(self, name):
        self.name = name

#decode_avmem_requirement_table for st7100/7109
decode_avmem_requirement_table_st7100 = {
    #hd_disp    hd_mpeg2    hd_h264  avmem_sys  avmem_vid
    (0,         0,          0):      (20*M,         0),
    (0,         1,          0):      (25*M,         0),
    (0,         0,          1):      (50*M,         0),
    (0,         1,          1):      (50*M,         0),
    
    (1,         0,          0):      (22*M,         0),
    (1,         1,          0):      (34*M,         0),
    (1,         0,          1):      (60*M,         0),
    (1,         1,          1):      (60*M,         0)
    }

#decode_avmem_requirement_table for st7111 & st7105
decode_avmem_requirement_table_st71xx = {
    #hd_disp    hd_mpeg2    hd_h264  avmem_sys  avmem_vid
    (0,         0,          0):      (20*M,         0),
    (0,         1,          0):      (25*M,         0),
    (0,         0,          1):      (50*M,         0),
    (0,         1,          1):      (50*M,         0),
    
    (1,         0,          0):      (22*M,         0),
    (1,         1,          0):      (34*M,         0),
    (1,         0,          1):      (60*M,         0),
    (1,         1,          1):      (60*M,         0)
    }

#decode_avmem_requirement_table for st5197
decode_avmem_requirement_table_st5197 = {
    #hd_disp    hd_mpeg2    hd_h264  avmem_sys  avmem_vid
    (0,         0,          0):      (6*M,         0)
    }

#platform dependent definition
#include 7100/7109/7101/5201/5203/5202
#only for 29 bits address mode
st7100_spec = {
    'PHYS_RAM_BASE': 0x04000000,
    'OFFSET_FOR_AV_FW': 0x400000,
    'LMI_VID_BASE':0xb0000000,
    'BOARD_MEM_MAP_NAME': 'st71xx_memory_map',
    'BOARD_MEM_LIB_NAME': 'mb411',
    'DECODE_AVMEM_REQUIREMENT_TABLE': decode_avmem_requirement_table_st7100
    }

st7109_spec = {
    'PHYS_RAM_BASE': 0x04000000,
    'OFFSET_FOR_AV_FW': 0x400000,
    'LMI_VID_BASE':0xb0000000,
    'BOARD_MEM_MAP_NAME': 'st71xx_memory_map',
    'BOARD_MEM_LIB_NAME': 'mb411stb7109',
    'DECODE_AVMEM_REQUIREMENT_TABLE': decode_avmem_requirement_table_st7100
    }

st7111_spec = {
    'PHYS_RAM_BASE': 0x0C000000,
    'OFFSET_FOR_AV_FW': 0x800000,
    'LMI_VID_BASE':0,
    'BOARD_MEM_MAP_NAME': 'st71xx_memory_map',
    'BOARD_MEM_LIB_NAME': 'mb618',
    'DECODE_AVMEM_REQUIREMENT_TABLE': decode_avmem_requirement_table_st71xx
    }
st7105_spec = {
    'PHYS_RAM_BASE': 0x0C000000,
    'OFFSET_FOR_AV_FW': 0x600000,
    'LMI_VID_BASE':0,
    'BOARD_MEM_MAP_NAME': 'st71xx_memory_map',
    'BOARD_MEM_LIB_NAME': 'mb680sti7105',
    'DECODE_AVMEM_REQUIREMENT_TABLE': decode_avmem_requirement_table_st71xx
    }
        
st5197_spec = {
    'PHYS_RAM_BASE': 0x0C000000,
    'OFFSET_FOR_AV_FW': 0x000000,
    'LMI_VID_BASE':0,
    'BOARD_MEM_MAP_NAME': 'st71xx_memory_map',
    'BOARD_MEM_LIB_NAME': 'mb704',
    'DECODE_AVMEM_REQUIREMENT_TABLE': decode_avmem_requirement_table_st5197
    }
    
st5206_spec = {
    'PHYS_RAM_BASE': 0x0C000000,
    'OFFSET_FOR_AV_FW': 0x500000,
    'LMI_VID_BASE':0,
    'BOARD_MEM_MAP_NAME': 'st71xx_memory_map',
    'BOARD_MEM_LIB_NAME': 'hdk5289stx5206',
    'DECODE_AVMEM_REQUIREMENT_TABLE': decode_avmem_requirement_table_st71xx
    }



platform_spec = None

validate_mode = False

# min size of System heap&stack
MIN_SYS_HEAP_STACK_SIZE = 2*M

#memory config from input
cfg_in = {
    #hardware config
    'DDR_BUS_NUMBER':1,
    'DDR0_TOTAL_SIZE':0x08000000,
    'DDR1_TOTAL_SIZE':0x00000000,
    #function options
    'HD_DISPLAY_SUPPORT':False,
    'HD_DECODER_SUPPORT':True,
    'HD_H264_SUPPORT':True,
    'PVR_SUPPORT':False,
    'PIP_SUPPORT':False,
    'OSD_BITS_PERPIEXL':32,
    #user defined size
    'SYSTEM_PARTITION_POSITION':0,
    'USER_OSD_AVMEM_SIZE':0x400000
    }

#memory partition define for output
cfg_out = {
    'LMI_SYS_BASE':0,
    'LMI_SYS_SIZE':0,
    'LMI_VID_BASE':0,
    'LMI_VID_SIZE':0,
    'APP_START_ADDR': 0,
    'STACK_ADDR': 0,
    'SYSTEM_PARTITIOAN_START_ADDR': 0,
    'SYSTEM_PARTITIOAN_SIZE': 0,
    'OS_PARTITIOAN_START_ADDR': 0,
    'OS_PARTITIOAN_SIZE': 0,
    'DRIVER_PARTITIOAN_START_ADDR':0,
    'DRIVER_PARTITIOAN_SIZE': 0,
    'NCACHE_PARTITIOAN_START_ADDR':0,
    'NCACHE_PARTITIOAN_SIZE': 0,
    'AVMEM_USER_START_ADDR': 0,
    'AVMEM_USER_SIZE': 0,
    'AVMEM_SYS_START_ADDR':0,
    'AVMEM_SYS_SIZE':0,
    'AVMEM_VID_START_ADDR':0,
    'AVMEM_VID_SIZE':0
    }

#args passed into this script
script_args = {
    'CFG_FILENAME':None,
    'CFG_SECTION':None,
    'MAP_FILENAME':None
    }

#for address translate
def phys_to_cached(addr):
    return long(addr)|0x80000000

def phys_to_uncached(addr):
    return long(addr)|0xA0000000

# workaround for ConfigParser reading HEX
def cfg_get_int(cfg, section, option):
    int_raw = cfg.get(section, option)
    int_s = str(int_raw).strip().lower()
    if int_s[0] == '0' and len(int_s) > 1 and int_s[1] == 'x':
        int_s = int_s.replace('0x', '')
        return int(int_s, 16)
    else:
        return int(int_raw)
        
def read_config():
    logging.debug('read config file...')
    cfg_filename = script_args['CFG_FILENAME']
    cfg_section = script_args['CFG_SECTION']
    
    logging.info('read [%s] from %s' % (cfg_section, cfg_filename))

    # read memory config
    config = ConfigParser.ConfigParser()
    config.read(cfg_filename)

    for k, v in cfg_in.iteritems():
        cfg_in[k] = cfg_get_int(config, cfg_section, k)
        
# do memory partition
def do_memory_partition():

    # TODO params check

    # display buffer size
    bytes_per_pixel = cfg_in['OSD_BITS_PERPIEXL']/8
    if bytes_per_pixel <= 0 or bytes_per_pixel > 4:
        raise Exception('bad value of OSD_BITS_PERPIEXL')
    
    osd_size = 720*576*bytes_per_pixel
    if cfg_in['HD_DISPLAY_SUPPORT']:
        osd_size += 1920*1080*bytes_per_pixel
    elif os.getenv('DVD_BACKEND') != '5197':
        osd_size += 720*576*bytes_per_pixel

    logging.debug('osd_size = %dK' % (osd_size/K))
    
    mem_key = (cfg_in['HD_DISPLAY_SUPPORT'], cfg_in['HD_DECODER_SUPPORT'], cfg_in['HD_H264_SUPPORT'])


    #get decode_avmem_requirement_table for special platform
    decode_avmem_requirement_table = platform_spec['DECODE_AVMEM_REQUIREMENT_TABLE']

    # decode size
    decode_size_lmi_sys = long(decode_avmem_requirement_table[mem_key][0])
    decode_size_lmi_vid = long(decode_avmem_requirement_table[mem_key][1])
    
    logging.debug('decode_size_lmi_sys = %dM' % (decode_size_lmi_sys/0x100000))
    logging.debug('decode_size_lmi_vid = %dM' % (decode_size_lmi_vid/0x100000))
    
    if cfg_in['PIP_SUPPORT']:
        decode_size_lmi_sys = decode_size_lmi_sys*2 - 2*M  #subtract audio used avmem (about 2M)
    
    ##             LMI_SYS
    ##          +--------------------------+  <--- _RAM_0_BASE
    ##          | 	VIDEO FIRMWARE	   |
    ##          +--------------------------+   
    ##          | 	AUDIO FIRMWARE	   |    
    ##          +--------------------------+           
    ##          | 		0x1000	   |
    ##          +--------------------------+ <--- PROGRAM_RAM_START
    ##          |                          |  
    ##          | 	    APP SPACE	   |
    ##          |       code_data_bss      |
    ##          |                          |      
    ##          +--------------------------+ 	
    ##          |                          |      
    ##          |   System Heap&Stack      |
    ##          |                          |
    ##          +--------------------------+ <--- SYSTEM_STACK_TOP_OFFSET
    ##          | os_partition             |        
    ##          +--------------------------+
    ##          | driver_partition         |
    ##          +--------------------------+   
    ##          | ncache_partition         |
    ##          +--------------------------+   
    ##          | avmem_user               |
    ##          +--------------------------+ 
    ##          | avmem_sys                |    
    ##          +--------------------------+ <--- _RAM_0_END
    ##
    ##             LMI_VID
    ##          +--------------------------+  <--- _RAM_1_BASE
    ##          | avmem_vid	           |
    ##          +--------------------------+   
    ##          | user_heap                |
    ##          | (system partition)       |    
    ##          +--------------------------+ <--- _RAM_1_END

    os_partition = Partition('os_partition')
    driver_partition = Partition('driver_partition')
    ncache_partition = Partition('ncache_partition')
    avmem_user = Partition('avmem_user')
    avmem_sys = Partition('avmem_sys')

    avmem_vid = Partition('avmem_vid')
    user_heap = Partition('user_heap')

    avmem_sys.size = decode_size_lmi_sys + osd_size
    avmem_vid.size = decode_size_lmi_vid
    avmem_user.size = cfg_in['USER_OSD_AVMEM_SIZE']

    is_user_heap_in_lmi_vid = (cfg_in['SYSTEM_PARTITION_POSITION'] != 0)

    # compute memory size of partitions first
    if not is_user_heap_in_lmi_vid:
        # system_heap (code_data_bss_end ~ stack) as user_heap_partition  
        user_heap.size = 0 
    else:
        user_heap.size = cfg_in['DDR1_TOTAL_SIZE'] - avmem_vid.size

    backend = os.getenv('DVD_BACKEND')

    if backend == '5197':
        #special case for st5197
        os_partition.size = 2.5*M
        driver_partition.size = 2*M
    elif backend == '7111' or backend == '7105':
        #default driver_partition size for basic case (st7111&st7105)
        os_partition.size = 4*M
        driver_partition.size = 3*M
    elif backend == '5206':
        #default driver_partition size for basic case (st5206)
        os_partition.size = 4*M
        driver_partition.size = 5*M
    elif backend == '7100' or backend == '7109':
        #default driver_partition size for basic case (st7100&st7109)
        os_partition.size = 2*M
        driver_partition.size = 2*M
    else:
        error_msg = 'unknown DVD backend! need conform!'
        logging.error(error_msg)
        raise Exception, error_msg
    
    if cfg_in['PIP_SUPPORT']:
        driver_partition.size += 3*M

    if  cfg_in['PVR_SUPPORT']:
        driver_partition.size += 8*M

    if cfg_in['PVR_SUPPORT']:
        ncache_partition.size = 0x800000 # Pvr need more ?
    else:
        ncache_partition.size = 0x80000
    
    start_addr_phys = platform_spec['PHYS_RAM_BASE'] + platform_spec['OFFSET_FOR_AV_FW'] + 0x1000

    #creat memory map of LMI_SYS
    partitions_in_lmi_sys = [os_partition, driver_partition, ncache_partition, avmem_user, avmem_sys]
    partitions_in_lmi_sys_size = 0
    partitions_count = len(partitions_in_lmi_sys)
    for i in range(partitions_count):
        partitions_in_lmi_sys_size += partitions_in_lmi_sys[i].size

    partitions_in_lmi_sys[0].base = platform_spec['PHYS_RAM_BASE'] + cfg_in['DDR0_TOTAL_SIZE'] - partitions_in_lmi_sys_size
    
    for i in range(1, partitions_count):
        partitions_in_lmi_sys[i].base = partitions_in_lmi_sys[i - 1].base + partitions_in_lmi_sys[i - 1].size

    # trace address info
    for i in range(0, partitions_count):
        logging.info('%-18s base(physical) = 0x%08x size = 0x%08x' % (partitions_in_lmi_sys[i].name, partitions_in_lmi_sys[i].base, partitions_in_lmi_sys[i].size))

    stack_addr_phys = partitions_in_lmi_sys[0].base - 4
    
    #creat memory map of LMI_VID
    assert avmem_vid.size <= cfg_in['DDR1_TOTAL_SIZE']
    avmem_vid.base = platform_spec['LMI_VID_BASE']
    if is_user_heap_in_lmi_vid:
        user_heap.base = avmem_vid.base + avmem_vid.size
        user_heap.size = cfg_in['DDR1_TOTAL_SIZE'] - avmem_vid.size
    else:
        # all of LMI_VID as avmem_vid
        avmem_vid.size = cfg_in['DDR1_TOTAL_SIZE']

    #validate
    if stack_addr_phys - start_addr_phys < MIN_SYS_HEAP_STACK_SIZE:
        logging.error('start_addr_phys = 0x%08x' % start_addr_phys)
        logging.error('stack_addr_phys = 0x%08x' % stack_addr_phys)
        raise Exception('App Memory Space is too low')

    #map file should be already create by LD
    if validate_mode:
        actual_start_addr, actual_end_addr = parse_code_data_memory_range()
        sys_heap_stack_size = phys_to_cached(stack_addr_phys) - actual_end_addr
        
        logging.info('code_data_bss start = 0x%08x'% actual_start_addr)
        logging.info('code_data_bss end   = 0x%08x'% actual_end_addr)
        logging.info('code_data_bss size  = 0x%08x'% (actual_end_addr - actual_start_addr))
        logging.info('stack_ptr           = 0x%08x'% phys_to_cached(stack_addr_phys))
        logging.info('sys_heap_stack_size = 0x%08x'% sys_heap_stack_size)
        
        if sys_heap_stack_size < MIN_SYS_HEAP_STACK_SIZE:
            raise Exception('sys_heap_stack_size is too low')

    cfg_out['LMI_SYS_BASE']                 = phys_to_uncached(platform_spec['PHYS_RAM_BASE'])
    cfg_out['LMI_SYS_SIZE']                 = cfg_in['DDR0_TOTAL_SIZE']
    cfg_out['LMI_VID_BASE']                 = platform_spec['LMI_VID_BASE']
    cfg_out['LMI_VID_SIZE']                 = cfg_in['DDR1_TOTAL_SIZE']
    cfg_out['APP_START_ADDR']               = phys_to_cached(start_addr_phys)
    cfg_out['STACK_ADDR']                   = phys_to_cached(stack_addr_phys)

    if is_user_heap_in_lmi_vid:
        #  memory in LMI_VID can be cacnhed from recent test from maocongwu
        cfg_out['SYSTEM_PARTITIOAN_START_ADDR'] = phys_to_cached(user_heap.base)
    else:
        cfg_out['SYSTEM_PARTITIOAN_START_ADDR'] = 0
    
    cfg_out['SYSTEM_PARTITIOAN_SIZE']           = user_heap.size
    cfg_out['OS_PARTITIOAN_START_ADDR']     = phys_to_cached(os_partition.base)
    cfg_out['OS_PARTITIOAN_SIZE']           = os_partition.size
    cfg_out['DRIVER_PARTITIOAN_START_ADDR']  = phys_to_cached(driver_partition.base)
    cfg_out['DRIVER_PARTITIOAN_SIZE']        = driver_partition.size
    cfg_out['NCACHE_PARTITIOAN_START_ADDR']  = phys_to_cached(ncache_partition.base)
    cfg_out['NCACHE_PARTITIOAN_SIZE']        = ncache_partition.size
    cfg_out['AVMEM_USER_START_ADDR']        = phys_to_uncached(avmem_user.base)
    cfg_out['AVMEM_USER_SIZE']              = avmem_user.size
    cfg_out['AVMEM_SYS_START_ADDR']         = phys_to_uncached(avmem_sys.base)
    cfg_out['AVMEM_SYS_SIZE']               = avmem_sys.size
    cfg_out['AVMEM_VID_START_ADDR']         = avmem_vid.base
    cfg_out['AVMEM_VID_SIZE']               = avmem_vid.size
    
# format cfg_out to output
def gen_cfg_h_file():
    logging.debug('generate head file...')
    out_filename = os.path.join(os.getenv('DVD_APPS'), 'memory_cfg.h')
    logging.info('creating %s ...' % out_filename)
    h_file = open(out_filename, 'w+')
    h_file.write('/* %s*/\n' % DONOT_MODIFY_NOTICE)
    h_file.write('#ifndef __MEMORY_CONFIG_H\n')
    h_file.write('#define __MEMORY_CONFIG_H\n\n')

    # sort dictionary items by name for output
    kv = []
    for k, v in cfg_in.iteritems():
        kv.append((k ,v))
        
    for k, v in sorted(kv):
        h_file.write('#define %s 0x%x\n'% (k, v))

    h_file.write('\n')
    
    kv = []
    for k, v in cfg_out.iteritems():
        kv.append((k ,v))
        
    for k, v in sorted(kv):
        h_file.write('#define %s 0x%x\n'% (k, v))
        

    h_file.write('\n#endif /* __MEMORY_CONFIG_H */ \n\n')
    h_file.write('\n/* End of File */\n')
    h_file.close()


def gen_board_mem_file():
    logging.debug('generate borad mem file...')
    #board_mem_filename = ''
    if os.getenv('MMCP_HOME') != None:
        out_filename = os.path.join(os.getenv('DVD_APPS'), 'board_' + os.getenv('DVD_BACKEND') + '.mem')
    else:
        out_filename = os.path.join(os.getenv('DVD_APPS'), 'board.mem')
    logging.info('creating %s ...' % out_filename)
    board_mem = open(out_filename, 'w+')
    board_mem.write('# %s\n\n' % DONOT_MODIFY_NOTICE)
    board_mem.write('*%s:\n'% platform_spec['BOARD_MEM_MAP_NAME'])
    board_mem.write('--defsym _start=0x%x --defsym _stack=0x%x\n\n'% (cfg_out['APP_START_ADDR'], cfg_out['STACK_ADDR']))
    board_mem.write('*board_link:\n')
    board_mem.write('%%{mboard=%s:%%(%s)}\n\n'% (platform_spec['BOARD_MEM_MAP_NAME'], platform_spec['BOARD_MEM_MAP_NAME']))
    board_mem.write('*lib_os21bsp_base:\n')
    board_mem.write('%%{mboard=%s:%s}\n'% (platform_spec['BOARD_MEM_MAP_NAME'], platform_spec['BOARD_MEM_LIB_NAME']))
    board_mem.close()

def gen_cfg_file_for_run_st7100():
    out_filename = os.path.join(os.getenv('DVD_CONFIG'), 'platform', os.getenv('DVD_BACKEND'), 'sys_mem_size.cmd')
    logging.info('creating %s ...' % out_filename)
    sys_mem_size_cmd = open(out_filename, 'w+')
    sys_mem_size_cmd.write('define stb%s_lmi_sys_mem_size_type\n'% os.getenv('DVD_BACKEND'))
    sys_mem_size_cmd.write('set $LMI_SYS_MEM_SIZE = 0x%x\n'%cfg_out['LMI_SYS_SIZE'])
    sys_mem_size_cmd.write('set $LMI_VID_MEM_SIZE = 0x%x\n'% cfg_out['LMI_VID_SIZE'])
    sys_mem_size_cmd.write('end\n\n')
    sys_mem_size_cmd.close()

def gen_cfg_file_for_run_st7111():
    out_filename = os.path.join(os.getenv('DVD_CONFIG'), 'platform', os.getenv('DVD_BACKEND'), 'targetpack', 'sys_mem_size.py')
    logging.info('creating %s ...' % out_filename)
    sys_mem_size_py = open(out_filename, 'w+')
    sys_mem_size_py.write('# %s\n' % DONOT_MODIFY_NOTICE)
    sys_mem_size_py.write('LMI_SYS_MEM_SIZE = 0x%x\n'%cfg_out['LMI_SYS_SIZE'])
    sys_mem_size_py.write('LMI_VID_MEM_SIZE = 0x%x\n'% cfg_out['LMI_VID_SIZE'])
    sys_mem_size_py.close()
    
#return (start, end) pair
def parse_code_data_memory_range():
    map_filename = os.path.join(script_args['MAP_FILENAME'])
    logging.debug('read map file %s' % map_filename)
    map_file = open(map_filename, 'r')
    text = map_file.read()
    map_file.close()
    
    m = re.search(r' {16}0x(?P<addr>\w+) {16}start', text)
    start = int(m.group('addr'), 16)
    
    m = re.search(r' {16}0x(?P<addr>\w+) {16}_end = \.', text)
    end = int(m.group('addr'), 16)

    return (start, end)
    
def toolset_check():
    logging.info('ST40 toolset checking...')
    st40versions = re.findall("ST40R(\d\.\d\.\d)", os.getenv('PATH'), re.IGNORECASE)
    #remove duplicate elements
    st40versions = list(set(st40versions))
    if len(st40versions) > 1:
        msg = ('different version of ST40(%s) toolset in PATH, environment setting maybe error!' % st40versions)
        logging.error(msg)
        raise Exception, msg
    if os.getenv('DVD_BACKEND') == '7105':
        #convert version string to tuple of int
        version_num = st40versions[0].split('.')
        version_num = (int(version_num[0]), int(version_num[1]), int(version_num[2]))
        #st40 for st7105 should be greater than ST40R4.4.0
        if version_num < (4, 4, 0):
            raise Exception, 'ST40 for st7105 should be equal or greater than ST40R4.4.0'
    logging.info('ST40R%s is OK' % st40versions[0])

generators = {
    '7100': {'PLATFORM_SPEC':st7100_spec,
             'GEN_STEPS':[read_config,do_memory_partition,gen_cfg_h_file,gen_board_mem_file,gen_cfg_file_for_run_st7100],
             'VALIDATE_STEPS':[read_config,do_memory_partition]},
    '7109': {'PLATFORM_SPEC':st7109_spec,
             'GEN_STEPS':[read_config,do_memory_partition,gen_cfg_h_file,gen_board_mem_file,gen_cfg_file_for_run_st7100],
             'VALIDATE_STEPS':[read_config,do_memory_partition]},
    '7111': {'PLATFORM_SPEC':st7111_spec,
             'GEN_STEPS':[read_config,do_memory_partition,gen_cfg_h_file,gen_board_mem_file,gen_cfg_file_for_run_st7111],
             'VALIDATE_STEPS':[read_config,do_memory_partition]},
    '7105': {'PLATFORM_SPEC':st7105_spec,
             'GEN_STEPS':[read_config,do_memory_partition,gen_cfg_h_file,gen_board_mem_file,gen_cfg_file_for_run_st7111],
             'VALIDATE_STEPS':[read_config,do_memory_partition]},
    '5197': {'PLATFORM_SPEC':st5197_spec,
             'GEN_STEPS':[read_config,do_memory_partition,gen_cfg_h_file,gen_board_mem_file,gen_cfg_file_for_run_st7111],
             'VALIDATE_STEPS':[read_config,do_memory_partition]},
    '5206': {'PLATFORM_SPEC':st5206_spec,
             'GEN_STEPS':[read_config,do_memory_partition,gen_cfg_h_file,gen_board_mem_file,gen_cfg_file_for_run_st7111],
             'VALIDATE_STEPS':[read_config,do_memory_partition]},           
    }

gen = generators[os.getenv('DVD_BACKEND')]
platform_spec = gen['PLATFORM_SPEC']

#setup logging
logging.basicConfig(level=logging.DEBUG, format="[MEMORY_CFG] %(message)s")
    
#usage: python memory_cfg.py CFG_FILENAME CFG_SECTION [MAP_FILENAME]
argc = len(sys.argv)

if argc < 3 or argc > 4:
    raise Exception('argment number error')
else:
    script_args['CFG_FILENAME'] = sys.argv[1]
    script_args['CFG_SECTION'] = sys.argv[2]

    if argc == 4:
            script_args['MAP_FILENAME'] = sys.argv[3]

# do steps
logging.info('version: %s' % VERSION)

toolset_check()

if script_args['MAP_FILENAME'] == None:
    logging.info('generate memory config files...')
    validate_mode = False
    steps = gen['GEN_STEPS']
else:
    logging.info('validate memory config...')
    validate_mode = True
    steps = gen['VALIDATE_STEPS']

for step in steps:
    step()

logging.info('exit memory_cfg.py...')

