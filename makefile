# CC Command
#
# Note: -O2 is sometimes broken in GCC when setjump/longjump is being
# used. Try -O2 only with released simulators.
#
CC = gcc -O2 -lm -I .
#CC = gcc -O2 -g -lm -I .



#
# Common Libraries
#
BIN = BIN/
SIM = scp.c scp_tty.c sim_sock.c sim_tmxr.c sim_ether.c



#
# Emulator source files and compile time options
#
PDP1D = PDP1/
PDP1 = ${PDP1D}pdp1_lp.c ${PDP1D}pdp1_cpu.c ${PDP1D}pdp1_stddev.c \
	${PDP1D}pdp1_sys.c ${PDP1D}pdp1_dt.c
PDP1_OPT = -I ${PDP1D}


NOVAD = NOVA/
NOVA = ${NOVAD}nova_sys.c ${NOVAD}nova_cpu.c ${NOVAD}nova_dkp.c \
	${NOVAD}nova_dsk.c ${NOVAD}nova_lp.c ${NOVAD}nova_mta.c \
	${NOVAD}nova_plt.c ${NOVAD}nova_pt.c ${NOVAD}nova_clk.c \
	${NOVAD}nova_tt.c ${NOVAD}nova_tt1.c
NOVA_OPT = -I ${NOVAD}



ECLIPSE = ${NOVAD}eclipse_cpu.c ${NOVAD}eclipse_tt.c ${NOVAD}nova_sys.c \
	${NOVAD}nova_dkp.c ${NOVAD}nova_dsk.c ${NOVAD}nova_lp.c \
	${NOVAD}nova_mta.c ${NOVAD}nova_plt.c ${NOVAD}nova_pt.c \
	${NOVAD}nova_clk.c ${NOVAD}nova_tt1.c
ECLIPSE_OPT = -I ${NOVAD} -DECLIPSE



PDP18BD = PDP18B/
PDP18B = ${PDP18BD}pdp18b_dt.c ${PDP18BD}pdp18b_drm.c ${PDP18BD}pdp18b_cpu.c \
	${PDP18BD}pdp18b_lp.c ${PDP18BD}pdp18b_mt.c ${PDP18BD}pdp18b_rf.c \
	${PDP18BD}pdp18b_rp.c ${PDP18BD}pdp18b_stddev.c ${PDP18BD}pdp18b_sys.c \
	${PDP18BD}pdp18b_tt1.c
PDP4_OPT = -DPDP4 -I ${PDP18BD}
PDP7_OPT = -DPDP7 -I ${PDP18BD}
PDP9_OPT = -DPDP9 -I ${PDP18BD}
PDP15_OPT = -DPDP15 -I ${PDP18BD}



PDP11D = PDP11/
PDP11 = ${PDP11D}pdp11_fp.c ${PDP11D}pdp11_cpu.c ${PDP11D}pdp11_dz.c \
	${PDP11D}pdp11_cis.c ${PDP11D}pdp11_lp.c ${PDP11D}pdp11_rk.c \
	${PDP11D}pdp11_rl.c ${PDP11D}pdp11_rp.c ${PDP11D}pdp11_rx.c \
	${PDP11D}pdp11_stddev.c ${PDP11D}pdp11_sys.c ${PDP11D}pdp11_tc.c \
	${PDP11D}pdp11_tm.c ${PDP11D}pdp11_ts.c ${PDP11D}pdp11_io.c \
	${PDP11D}pdp11_rq.c ${PDP11D}pdp11_tq.c ${PDP11D}pdp11_pclk.c \
	${PDP11D}pdp11_ry.c ${PDP11D}pdp11_pt.c ${PDP11D}pdp11_hk.c \
	${PDP11D}pdp11_xq.c
PDP11_OPT = -I ${PDP11D}



VAXD = VAX/
VAX = ${VAXD}vax_cpu1.c ${VAXD}vax_cpu.c ${VAXD}vax_fpa.c ${VAXD}vax_io.c \
	${VAXD}vax_mmu.c ${VAXD}vax_stddev.c ${VAXD}vax_sys.c \
	${VAXD}vax_sysdev.c \
	${PDP11D}pdp11_rl.c ${PDP11D}pdp11_rq.c ${PDP11D}pdp11_ts.c \
	${PDP11D}pdp11_dz.c ${PDP11D}pdp11_lp.c ${PDP11D}pdp11_tq.c \
	${PDP11D}pdp11_pt.c ${PDP11D}pdp11_xq.c
VAX_OPT = -I ${VAXD} -I ${PDP11D} -DUSE_INT64



PDP10D = PDP10/
PDP10 = ${PDP10D}pdp10_fe.c ${PDP10D}pdp10_dz.c ${PDP10D}pdp10_cpu.c \
	${PDP10D}pdp10_ksio.c ${PDP10D}pdp10_lp20.c ${PDP10D}pdp10_mdfp.c \
	${PDP10D}pdp10_pag.c ${PDP10D}pdp10_rp.c ${PDP10D}pdp10_sys.c \
	${PDP10D}pdp10_tim.c ${PDP10D}pdp10_tu.c ${PDP10D}pdp10_xtnd.c \
	${PDP10D}pdp10_pt.c ${PDP11D}pdp11_ry.c
PDP10_OPT = -DUSE_INT64 -I ${PDP10D} -I ${PDP11D} -I ${VAXD}



PDP8D = PDP8/
PDP8 = ${PDP8D}pdp8_cpu.c ${PDP8D}pdp8_clk.c ${PDP8D}pdp8_df.c \
	${PDP8D}pdp8_dt.c ${PDP8D}pdp8_lp.c ${PDP8D}pdp8_mt.c \
	${PDP8D}pdp8_pt.c ${PDP8D}pdp8_rf.c ${PDP8D}pdp8_rk.c \
	${PDP8D}pdp8_rx.c ${PDP8D}pdp8_sys.c ${PDP8D}pdp8_tt.c \
	${PDP8D}pdp8_ttx.c ${PDP8D}pdp8_rl.c
PDP8_OPT = -I ${PDP8D}



H316D = H316/
H316 = ${H316D}h316_stddev.c ${H316D}h316_lp.c ${H316D}h316_cpu.c \
	${H316D}h316_sys.c
H316_OPT = -I ${H316D}



HP2100D = HP2100/
HP2100 = ${HP2100D}hp2100_stddev.c ${HP2100D}hp2100_dp.c ${HP2100D}hp2100_dq.c \
	${HP2100D}hp2100_dr.c ${HP2100D}hp2100_lps.c ${HP2100D}hp2100_ms.c \
	${HP2100D}hp2100_mt.c ${HP2100D}hp2100_mux.c ${HP2100D}hp2100_cpu.c \
	${HP2100D}hp2100_fp.c ${HP2100D}hp2100_sys.c ${HP2100D}hp2100_lpt.c
HP2100_OPT = -I ${HP2100D}



I1401D = I1401/
I1401 = ${I1401D}i1401_lp.c ${I1401D}i1401_cpu.c ${I1401D}i1401_iq.c \
	${I1401D}i1401_cd.c ${I1401D}i1401_mt.c ${I1401D}i1401_dp.c \
	${I1401D}i1401_sys.c
I1401_OPT = -I ${I1401D}



I1620D = I1620/
I1620 = ${I1620D}i1620_cd.c ${I1620D}i1620_dp.c ${I1620D}i1620_pt.c \
	${I1620D}i1620_tty.c ${I1620D}i1620_cpu.c ${I1620D}i1620_lp.c \
	${I1620D}i1620_fp.c ${I1620D}i1620_sys.c
I1620_OPT = -I ${I1620D}



IBM1130D = Ibm1130/
IBM1130 = ${IBM1130D}ibm1130_cpu.c ${IBM1130D}ibm1130_cr.c \
	${IBM1130D}ibm1130_disk.c ${IBM1130D}ibm1130_stddev.c \
	${IBM1130D}ibm1130_sys.c ${IBM1130D}ibm1130_gdu.c \
	${IBM1130D}ibm1130_gui.c ${IBM1130D}ibm1130_prt.c
IBM1130_OPT = -I ${IBM1130D}



ID16D = Interdata/
ID16 = ${ID16D}id16_cpu.c ${ID16D}id16_sys.c ${ID16D}id_dp.c \
	${ID16D}id_fd.c ${ID16D}id_fp.c ${ID16D}id_idc.c ${ID16D}id_io.c \
	${ID16D}id_lp.c ${ID16D}id_mt.c ${ID16D}id_pas.c ${ID16D}id_pt.c \
	${ID16D}id_tt.c ${ID16D}id_uvc.c
ID16_OPT = -I ${ID16D}



ID32D = Interdata/
ID32 = ${ID32D}id32_cpu.c ${ID32D}id32_sys.c ${ID32D}id_dp.c \
	${ID32D}id_fd.c ${ID32D}id_fp.c ${ID32D}id_idc.c ${ID32D}id_io.c \
	${ID32D}id_lp.c ${ID32D}id_mt.c ${ID32D}id_pas.c ${ID32D}id_pt.c \
	${ID32D}id_tt.c ${ID32D}id_uvc.c
ID32_OPT = -I ${ID32D}



S3D = S3/
S3 = ${S3D}s3_cd.c ${S3D}s3_cpu.c ${S3D}s3_disk.c ${S3D}s3_lp.c \
	${S3D}s3_pkb.c ${S3D}s3_sys.c
S3_OPT = -I ${S3D}



ALTAIRD = ALTAIR/
ALTAIR = ${ALTAIRD}altair_sio.c ${ALTAIRD}altair_cpu.c ${ALTAIRD}altair_dsk.c \
	${ALTAIRD}altair_sys.c
ALTAIR_OPT = -I ${ALTAIRD}



ALTAIRZ80D = AltairZ80/
ALTAIRZ80 = ${ALTAIRZ80D}altairz80_cpu.c ${ALTAIRZ80D}altairz80_dsk.c \
	${ALTAIRZ80D}altairz80_sio.c ${ALTAIRZ80D}altairz80_sys.c \
	${ALTAIRZ80D}altairz80_hdsk.c
ALTAIRZ80_OPT = -I ${ALTAIRZ80D}



GRID = GRI/
GRI = ${GRID}gri_cpu.c ${GRID}gri_stddev.c ${GRID}gri_sys.c
GRI_OPT = -I ${GRID}



SDSD = SDS/
SDS = ${SDSD}sds_cpu.c ${SDSD}sds_drm.c ${SDSD}sds_dsk.c ${SDSD}sds_io.c \
	${SDSD}sds_lp.c ${SDSD}sds_mt.c ${SDSD}sds_mux.c ${SDSD}sds_rad.c \
	${SDSD}sds_stddev.c ${SDSD}sds_sys.c
SDS_OPT = -I ${SDSD}



#
# Build everything
#
all : ${BIN}pdp1 ${BIN}pdp4 ${BIN}pdp7 ${BIN}pdp8 ${BIN}pdp9 ${BIN}pdp15 \
	${BIN}pdp11 ${BIN}pdp10 ${BIN}vax ${BIN}nova ${BIN}eclipse ${BIN}h316 \
	${BIN}hp2100 ${BIN}i1401 ${BIN}i1620 ${BIN}s3 ${BIN}altair \
	${BIN}altairz80 ${BIN}gri ${BIN}i1620 ${BIN}ibm1130 \
	${BIN}id16 ${BIN}id32 ${BIN}sds



#
# Individual builds
#
${BIN}pdp1 : ${PDP1} ${SIM}
	${CC} ${PDP1} ${SIM} ${PDP1_OPT} -o $@



${BIN}pdp4 : ${PDP18B} ${SIM}
	${CC} ${PDP18B} ${SIM} ${PDP4_OPT} -o $@



${BIN}pdp7 : ${PDP18B} ${SIM}
	${CC} ${PDP18B} ${SIM} ${PDP7_OPT} -o $@



${BIN}pdp8 : ${PDP8} ${SIM}
	${CC} ${PDP8} ${SIM} ${PDP8_OPT} -o $@



${BIN}pdp9 : ${PDP18B} ${SIM}
	${CC} ${PDP18B} ${SIM} ${PDP9_OPT} -o $@



${BIN}pdp15 : ${PDP18B} ${SIM}
	${CC} ${PDP18B} ${SIM} ${PDP15_OPT} -o $@



${BIN}pdp10 : ${PDP10} ${SIM}
	${CC} ${PDP10} ${SIM} ${PDP10_OPT} -o $@



${BIN}pdp11 : ${PDP11} ${SIM}
	${CC} ${PDP11} ${SIM} ${PDP11_OPT} -o $@



${BIN}vax : ${VAX} ${SIM}
	${CC} ${VAX} ${SIM} ${VAX_OPT} -o $@



${BIN}nova : ${NOVA} ${SIM}
	${CC} ${NOVA} ${SIM} ${NOVA_OPT} -o $@



${BIN}eclipse : ${ECLIPSE} ${SIM}
	${CC} ${ECLIPSE} ${SIM} ${ECLIPSE_OPT} -o $@



${BIN}h316 : ${H316} ${SIM}
	${CC} ${H316} ${SIM} ${H316_OPT} -o $@



${BIN}hp2100 : ${HP2100} ${SIM}
	${CC} ${HP2100} ${SIM} ${HP2100_OPT} -o $@



${BIN}i1401 : ${I1401} ${SIM}
	${CC} ${I1401} ${SIM} ${I1401_OPT} -o $@



${BIN}i1620 : ${I1620} ${SIM}
	${CC} ${I1620} ${SIM} ${I1620_OPT} -o $@


${BIN}ibm1130 : ${IBM1130}
	${CC} ${IBM1130} ${SIM} ${IBM1130_OPT} -o $@


${BIN}s3 : ${S3} ${SIM}
	${CC} ${S3} ${SIM} ${S3_OPT} -o $@


${BIN}altair : ${ALTAIR} ${SIM}
	${CC} ${ALTAIR} ${SIM} ${ALTAIR_OPT} -o $@


${BIN}altairz80 : ${ALTAIRZ80} ${SIM} ${ALTAIRZ80D}altairZ80_defs.h
	${CC} ${ALTAIRZ80_OPT} ${ALTAIRZ80} ${SIM} -o $@
${ALTAIRZ80D}altairZ80_defs.h : ${ALTAIRZ80D}altairz80_defs.h
	cp ${ALTAIRZ80D}altairz80_defs.h ${ALTAIRZ80D}altairZ80_defs.h

${BIN}gri : ${GRI} ${SIM}
	${CC} ${GRI} ${SIM} ${GRI_OPT} -o $@

${BIN}id16 : ${ID16} ${SIM}
	${CC} ${ID16} ${SIM} ${ID16_OPT} -o $@

${BIN}id32 : ${ID32} ${SIM}
	${CC} ${ID32} ${SIM} ${ID32_OPT} -o $@

${BIN}sds : ${SDS} ${SIM}
	${CC} ${SDS} ${SIM} ${SDS_OPT} -o $@


