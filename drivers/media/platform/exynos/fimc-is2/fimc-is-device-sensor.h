/*
 * Samsung Exynos5 SoC series FIMC-IS driver
 *
 *
 * Copyright (c) 2011 Samsung Electronics Co., Ltd
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#ifndef fimc_is_device_sensor_H
#define fimc_is_device_sensor_H

#include <mach/exynos-fimc-is2-sensor.h>
#include "fimc-is-mem.h"
#include "fimc-is-video.h"
#include "fimc-is-resourcemgr.h"
#include "fimc-is-device-flite.h"
#include "fimc-is-device-csi.h"

struct fimc_is_video_ctx;
struct fimc_is_device_ischain;

#define SENSOR_MAX_ENUM			SENSOR_NAME_END
#define SENSOR_DEFAULT_FRAMERATE	30

#define SENSOR_SCENARIO_MASK		0xF0000000
#define SENSOR_SCENARIO_SHIFT		28
#define SENSOR_MODULE_MASK		0x0FFFFFFF
#define SENSOR_MODULE_SHIFT		0

#define SENSOR_SSTREAM_MASK		0x0000000F
#define SENSOR_SSTREAM_SHIFT		0
#define SENSOR_INSTANT_MASK		0x0FFF0000
#define SENSOR_INSTANT_SHIFT		16
#define SENSOR_NOBLOCK_MASK		0xF0000000
#define SENSOR_NOBLOCK_SHIFT		28

#define SENSOR_I2C_CH_MASK		0xFF
#define SENSOR_I2C_CH_SHIFT		0
#define ACTUATOR_I2C_CH_MASK		0xFF00
#define ACTUATOR_I2C_CH_SHIFT		8
#define OIS_I2C_CH_MASK 		0xFF0000
#define OIS_I2C_CH_SHIFT		16

#define SENSOR_I2C_ADDR_MASK		0xFF
#define SENSOR_I2C_ADDR_SHIFT		0
#define ACTUATOR_I2C_ADDR_MASK		0xFF00
#define ACTUATOR_I2C_ADDR_SHIFT		8
#define OIS_I2C_ADDR_MASK		0xFF0000
#define OIS_I2C_ADDR_SHIFT		16

#define FIMC_IS_SENSOR_CFG(w, h, f, s, m) {	\
	.width		= w,			\
	.height		= h,			\
	.framerate	= f,			\
	.settle		= s,			\
	.mode		= m,			\
}

enum fimc_is_sensor_output_entity {
	FIMC_IS_SENSOR_OUTPUT_NONE = 0,
	FIMC_IS_SENSOR_OUTPUT_FRONT,
};

enum fimc_is_sensor_force_stop {
	FIMC_IS_BAD_FRAME_STOP = 0,
	FIMC_IS_MIF_THROTTLING_STOP = 1,
	FIMC_IS_FLITE_OVERFLOW_STOP = 2
};

enum fimc_is_module_state {
	FIMC_IS_MODULE_GPIO_ON
};

struct fimc_is_sensor_cfg {
	u32 width;
	u32 height;
	u32 framerate;
	u32 settle;
	int mode;
};

struct fimc_is_sensor_ops {
	int (*stream_on)(struct v4l2_subdev *subdev);
	int (*stream_off)(struct v4l2_subdev *subdev);

	int (*s_duration)(struct v4l2_subdev *subdev, u64 duration);
	int (*g_min_duration)(struct v4l2_subdev *subdev);
	int (*g_max_duration)(struct v4l2_subdev *subdev);

	int (*s_exposure)(struct v4l2_subdev *subdev, u64 exposure);
	int (*g_min_exposure)(struct v4l2_subdev *subdev);
	int (*g_max_exposure)(struct v4l2_subdev *subdev);

	int (*s_again)(struct v4l2_subdev *subdev, u64 sensivity);
	int (*g_min_again)(struct v4l2_subdev *subdev);
	int (*g_max_again)(struct v4l2_subdev *subdev);

	int (*s_dgain)(struct v4l2_subdev *subdev);
	int (*g_min_dgain)(struct v4l2_subdev *subdev);
	int (*g_max_dgain)(struct v4l2_subdev *subdev);
};

struct fimc_is_module_enum {
	u32						sensor_id;
	struct v4l2_subdev				*subdev; /* connected module subdevice */
	u32						device; /* connected sensor device */
	unsigned long					state;
	u32						pixel_width;
	u32						pixel_height;
	u32						active_width;
	u32						active_height;
	u32						max_framerate;
	u32						position;
	u32						mode;
	u32						lanes;
	u32						vcis;
	struct fimc_is_vci				*vci;
	u32						cfgs;
	struct fimc_is_sensor_cfg			*cfg;
	struct i2c_client				*client;
	struct sensor_open_extended			ext;
	struct fimc_is_sensor_ops			*ops;
	char						*sensor_maker;
	char						*sensor_name;
	char						*setfile_name;
	void						*private_data;
	struct exynos_platform_fimc_is_module		*pdata;
	struct platform_device				*pdev;
};

enum fimc_is_sensor_state {
	FIMC_IS_SENSOR_OPEN,
	FIMC_IS_SENSOR_MCLK_ON,
	FIMC_IS_SENSOR_ICLK_ON,
	FIMC_IS_SENSOR_GPIO_ON,
	FIMC_IS_SENSOR_POWER_ON,
	FIMC_IS_SENSOR_S_INPUT,
	FIMC_IS_SENSOR_DRIVING,
	FIMC_IS_SENSOR_FRONT_START,
	FIMC_IS_SENSOR_FRONT_DTP_STOP,
	FIMC_IS_SENSOR_BACK_START,
	FIMC_IS_SENSOR_BACK_NOWAIT_STOP
};

struct fimc_is_device_sensor {
	struct v4l2_device				v4l2_dev;
	struct platform_device				*pdev;
	struct fimc_is_mem				mem;

	u32						instance;
	u32						position;
	struct fimc_is_image				image;

	struct fimc_is_video_ctx			*vctx;
	struct fimc_is_video				video;

	struct fimc_is_device_ischain   		*ischain;
	struct fimc_is_resourcemgr			*resourcemgr;
	struct fimc_is_module_enum			module_enum[SENSOR_MAX_ENUM];

	/* current control value */
	struct camera2_sensor_ctl			sensor_ctl;
	struct camera2_lens_ctl				lens_ctl;
	struct camera2_flash_ctl			flash_ctl;
	struct work_struct				control_work;
	struct fimc_is_frame				*control_frame;

	u32						fcount;
	u32						instant_cnt;
	int						instant_ret;
	wait_queue_head_t				instant_wait;
	struct work_struct				instant_work;
	unsigned long					state;
	spinlock_t					slock_state;

	/* hardware configuration */
	struct v4l2_subdev				*subdev_module;
	struct v4l2_subdev				*subdev_csi;
	struct v4l2_subdev				*subdev_flite;

	int						mode;
	/* gain boost */
	int						min_target_fps;
	int						max_target_fps;
	int						scene_mode;

	/* for vision control */
	int						exposure_time;
	u64						frame_duration;

	/* ENABLE_DTP */
	bool						dtp_check;
	struct timer_list				dtp_timer;
	unsigned long					force_stop;

	/* Actuator Init Skip */
	bool                                    actuator_init;

	struct exynos_platform_fimc_is_sensor		*pdata;
	void						*private_data;
};

int fimc_is_sensor_open(struct fimc_is_device_sensor *device,
	struct fimc_is_video_ctx *vctx);
int fimc_is_sensor_close(struct fimc_is_device_sensor *device);
int fimc_is_sensor_s_input(struct fimc_is_device_sensor *device,
	u32 input,
	u32 scenario);
int fimc_is_sensor_s_ctrl(struct fimc_is_device_sensor *device,
	struct v4l2_control *ctrl);
int fimc_is_sensor_buffer_queue(struct fimc_is_device_sensor *device,
	u32 index);
int fimc_is_sensor_buffer_finish(struct fimc_is_device_sensor *device,
	u32 index);

int fimc_is_sensor_front_start(struct fimc_is_device_sensor *device,
	u32 instant_cnt,
	u32 nonblock);
int fimc_is_sensor_front_stop(struct fimc_is_device_sensor *device);

int fimc_is_sensor_s_framerate(struct fimc_is_device_sensor *device,
	struct v4l2_streamparm *param);
int fimc_is_sensor_s_bns(struct fimc_is_device_sensor *device,
	u32 reatio);

int fimc_is_sensor_s_frame_duration(struct fimc_is_device_sensor *device,
	u32 frame_duration);
int fimc_is_sensor_s_exposure_time(struct fimc_is_device_sensor *device,
	u32 exposure_time);

int fimc_is_sensor_gpio_on(struct fimc_is_device_sensor *device);
int fimc_is_sensor_gpio_off(struct fimc_is_device_sensor *device);
int fimc_is_sensor_gpio_dbg(struct fimc_is_device_sensor *device);

int fimc_is_sensor_g_ctrl(struct fimc_is_device_sensor *device,
	struct v4l2_control *ctrl);
int fimc_is_sensor_g_instance(struct fimc_is_device_sensor *device);
int fimc_is_sensor_g_framerate(struct fimc_is_device_sensor *device);
int fimc_is_sensor_g_fcount(struct fimc_is_device_sensor *device);
int fimc_is_sensor_g_width(struct fimc_is_device_sensor *device);
int fimc_is_sensor_g_height(struct fimc_is_device_sensor *device);
int fimc_is_sensor_g_bns_width(struct fimc_is_device_sensor *device);
int fimc_is_sensor_g_bns_height(struct fimc_is_device_sensor *device);
int fimc_is_sensor_g_bns_ratio(struct fimc_is_device_sensor *device);
int fimc_is_sensor_g_bratio(struct fimc_is_device_sensor *device);
int fimc_is_sensor_g_module(struct fimc_is_device_sensor *device,
	struct fimc_is_module_enum **module);
int fimc_is_sensor_g_postion(struct fimc_is_device_sensor *device);
int fimc_is_search_sensor_module(struct fimc_is_device_sensor *device,
	u32 sensor_id, struct fimc_is_module_enum **module);
int fimc_is_sensor_runtime_suspend(struct device *dev);
int fimc_is_sensor_runtime_resume(struct device *dev);

extern const struct fimc_is_queue_ops fimc_is_sensor_ops;

#define CALL_MOPS(s, op, args...) (((s)->ops->op) ? ((s)->ops->op(args)) : 0)

#endif
