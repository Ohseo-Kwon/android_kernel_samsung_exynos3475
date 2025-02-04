/*
 * Samsung EXYNOS FIMC-IS (Imaging Subsystem) driver
 *
 * Copyright (C) 2015 Samsung Electronics Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/kernel.h>

#include <linux/videodev2.h>
#include <linux/videodev2_exynos_camera.h>

#include "fimc-is-interface-sensor.h"
#include "fimc-is-control-sensor.h"
#include "fimc-is-device-sensor.h"
#include "fimc-is-device-sensor-peri.h"

int set_actuator_position_table(struct fimc_is_sensor_interface *itf,
				u32 *position_table)
{
	int ret = 0;
	int i;
	struct fimc_is_actuator *actuator = NULL;
	struct fimc_is_actuator_interface *actuator_itf = NULL;
	struct fimc_is_device_sensor_peri *sensor_peri = NULL;

	BUG_ON(!itf);
	BUG_ON(!position_table);
	BUG_ON(itf->magic != SENSOR_INTERFACE_MAGIC);

	actuator = get_subdev_actuator(itf);
	BUG_ON(!actuator);

	actuator_itf = &itf->actuator_itf;
	BUG_ON(!actuator_itf);

	sensor_peri = container_of(itf, struct fimc_is_device_sensor_peri, sensor_interface);
	BUG_ON(!sensor_peri);

	if (!test_bit(FIMC_IS_SENSOR_ACTUATOR_AVAILABLE, &sensor_peri->peri_state)) {
		dbg_sen_itf("%s: FIMC_IS_SENSOR_ACTUATOR_NOT_AVAILABLE\n", __func__);
		goto p_err;
	}

	for (i = 0; i < ACTUATOR_MAX_FOCUS_POSITIONS; i++) {
		actuator_itf->position_table.hw_table[i] = position_table[i];
		ret = fimc_is_actuator_ctl_convert_position(&actuator_itf->position_table.hw_table[i],
					ACTUATOR_POS_SIZE_10BIT,
					ACTUATOR_RANGE_INF_TO_MAC,
					actuator->pos_size_bit,
					actuator->pos_direction);
		if (ret) {
			err("Converted position fail.(%d)\n", ret);
			goto p_err;
		}
		dbg_sen_itf("%s: converted virtual position(%d) -> hw_position (%d)\n",
				__func__,
				i, actuator_itf->position_table.hw_table[i]);
	}
	actuator_itf->position_table.enable = true;

	if (!actuator_itf->initialized) {
		ret = fimc_is_actuator_ctl_search_position(actuator->position,
						actuator_itf->position_table.hw_table,
						actuator->pos_direction,
						&actuator_itf->virtual_pos);
		if (ret) {
			err("Searched position fail.(%d)\n", ret);
			goto p_err;
		}

		actuator_itf->initialized = true;
		dbg_sen_itf("%s: initial actuator position: %d, algorithm position: %d\n",
				__func__, actuator->position, actuator_itf->virtual_pos);
	}

p_err:

	return ret;
}

int set_soft_landing_config(struct fimc_is_sensor_interface *itf,
				u32 step_delay,
				u32 position_num,
				u32 *position_table)
{
	int ret = 0;
	u32 cnt = 0, max_hw_pos = 0, converted_pos = 0;
	struct fimc_is_actuator *actuator = NULL;
	struct fimc_is_actuator_interface *actuator_itf = NULL;
	struct fimc_is_device_sensor_peri *sensor_peri = NULL;

	if (unlikely(!itf)) {
		err("%s, interface in is NULL", __func__);
		ret = -EINVAL;
		goto p_err;
	}

	BUG_ON(!position_table);
	BUG_ON(itf->magic != SENSOR_INTERFACE_MAGIC);

	actuator = get_subdev_actuator(itf);
	BUG_ON(!actuator);

	actuator_itf = &itf->actuator_itf;
	BUG_ON(!actuator_itf);

	sensor_peri = container_of(itf, struct fimc_is_device_sensor_peri, sensor_interface);
	BUG_ON(!sensor_peri);

	if (!test_bit(FIMC_IS_SENSOR_ACTUATOR_AVAILABLE, &sensor_peri->peri_state)) {
		dbg_sen_itf("%s: FIMC_IS_SENSOR_ACTUATOR_NOT_AVAILABLE\n", __func__);
		goto p_err;
	}

	if (position_num > ACTUATOR_MAX_SOFT_LANDING_NUM) {
		err("Position number is bigger than %d\n", ACTUATOR_MAX_SOFT_LANDING_NUM);
		ret = -EINVAL;
		goto p_err;
	}

	actuator_itf->soft_landing_table.step_delay = step_delay;
	actuator_itf->soft_landing_table.position_num = position_num;

	memset(actuator_itf->soft_landing_table.virtual_table, 0, sizeof(u32 [ACTUATOR_MAX_SOFT_LANDING_NUM]));
	memset(actuator_itf->soft_landing_table.hw_table, 0, sizeof(u32 [ACTUATOR_MAX_SOFT_LANDING_NUM]));

	max_hw_pos = (1 << actuator->pos_size_bit);

	for (cnt = 0; cnt < position_num; cnt++) {
		/* Return invalid */
		if (position_table[cnt] >= max_hw_pos) {
			err("Invalid position, max: %d\n", max_hw_pos);
			ret = -EINVAL;
			goto p_err;
		}

		actuator_itf->soft_landing_table.hw_table[cnt] = position_table[cnt];
		converted_pos = actuator_itf->soft_landing_table.hw_table[cnt];

		ret = fimc_is_actuator_ctl_convert_position(&converted_pos,
					actuator->pos_size_bit,
					actuator->pos_direction,
					ACTUATOR_POS_SIZE_10BIT,
					ACTUATOR_RANGE_INF_TO_MAC);
		if (ret) {
			err("Converted position fail.(%d)\n", ret);
			goto p_err;
		}

		/* H/W position -> AF position */
		actuator_itf->soft_landing_table.virtual_table[cnt]
					= converted_pos;
	}

	actuator_itf->soft_landing_table.enable = true;
p_err:
	return ret;
}

int set_position(struct fimc_is_sensor_interface *itf, u32 position)
{
	int ret = 0;
	u32 max_real_pos = (1 << ACTUATOR_POS_SIZE_10BIT);
	struct fimc_is_device_sensor *device = NULL;
	struct fimc_is_module_enum *module = NULL;
	struct v4l2_subdev *subdev_module = NULL;
	struct fimc_is_device_sensor_peri *sensor_peri = NULL;
	struct fimc_is_actuator_interface *actuator_itf = NULL;
	struct fimc_is_actuator *actuator = NULL;

	BUG_ON(!itf);
	BUG_ON(itf->magic != SENSOR_INTERFACE_MAGIC);

	sensor_peri = container_of(itf, struct fimc_is_device_sensor_peri, sensor_interface);
	BUG_ON(!sensor_peri);

	if (!test_bit(FIMC_IS_SENSOR_ACTUATOR_AVAILABLE, &sensor_peri->peri_state)) {
		dbg_sen_itf("%s: FIMC_IS_SENSOR_ACTUATOR_NOT_AVAILABLE\n", __func__);
		position = 0;
		goto p_err;
	}

	module = sensor_peri->module;
	BUG_ON(!module);

	subdev_module = module->subdev;
	BUG_ON(!subdev_module);

	device = v4l2_get_subdev_hostdata(subdev_module);
	BUG_ON(!device);

	actuator_itf = &itf->actuator_itf;
	BUG_ON(!actuator_itf);

	actuator = get_subdev_actuator(itf);
	BUG_ON(!actuator);

	actuator_itf->virtual_pos = position;

	/* Return invalid */
	if (position >= max_real_pos) {
		err("Invalid position, max: %d\n", max_real_pos);
		ret = -EINVAL;
		goto p_err;
	}

	if (itf->actuator_itf.position_table.enable) {
		dbg_sen_itf("%s: converted virtual position(%d) -> hw_position (%d)\n",
				__func__, position,
				actuator_itf->position_table.hw_table[position]);
		position = actuator_itf->position_table.hw_table[position];
	} else {
		ret = fimc_is_actuator_ctl_convert_position(&position,
				ACTUATOR_POS_SIZE_10BIT,
				ACTUATOR_RANGE_INF_TO_MAC,
				actuator->pos_size_bit,
				actuator->pos_direction);
	}

	if (ret) {
		err("Converted position fail.(%d)\n", ret);
		goto p_err;
	}

	ret = fimc_is_actuator_ctl_set_position(device, position);
	if (ret < 0) {
		err("err!!! ret(%d), invalid position(%d)",
				ret, position);
		ret = -EINVAL;
		goto p_err;
	}
	actuator_itf->hw_pos = position;

p_err:
	return ret;
}

/*
 * AF algorithm need a real position at cur frame.
 * Consider to the frame delay in M2M scenario.
 * Because algorithm frame and sensor frame are different by M2M delay.
 * This get_cur_frame_position is frame position for AF algorithm view.
 */
int get_cur_frame_position(struct fimc_is_sensor_interface *itf, u32 *position)
{
	int ret = 0;
	u32 cur_frame_cnt = 0, index = 0;
	struct fimc_is_actuator_interface *actuator_itf = NULL;
	struct fimc_is_device_sensor_peri *sensor_peri = NULL;

	if (unlikely(!itf)) {
		err("%s, interface in is NULL", __func__);
		ret = -EINVAL;
		goto p_err;
	}

	BUG_ON(!position);
	BUG_ON(itf->magic != SENSOR_INTERFACE_MAGIC);

	sensor_peri = container_of(itf, struct fimc_is_device_sensor_peri, sensor_interface);
	BUG_ON(!sensor_peri);

	if (!test_bit(FIMC_IS_SENSOR_ACTUATOR_AVAILABLE, &sensor_peri->peri_state)) {
		dbg_sen_itf("%s: FIMC_IS_SENSOR_ACTUATOR_NOT_AVAILABLE\n", __func__);
		*position = 0;
		goto p_err;
	}

	actuator_itf = &itf->actuator_itf;
	BUG_ON(!actuator_itf);

	cur_frame_cnt = get_frame_count(itf);
	index = cur_frame_cnt % EXPECT_DM_NUM;
	*position = sensor_peri->cis.expecting_lens_udm[index].pos;

	dbg_sen_itf("%s: cur_frame(%d), position(%d)\n",
			__func__, cur_frame_cnt, *position);

p_err:

	return ret;
}

/*
 * Get actuator actual position.
 * The position is applied actuator.
 */
int get_applied_actual_position(struct fimc_is_sensor_interface *itf, u32 *position)
{
	int ret = 0;
	struct fimc_is_actuator_interface *actuator_itf = NULL;
	struct fimc_is_device_sensor_peri *sensor_peri = NULL;

	if (unlikely(!itf)) {
		err("%s, interface in is NULL", __func__);
		ret = -EINVAL;
		goto p_err;
	}

	BUG_ON(!position);
	BUG_ON(itf->magic != SENSOR_INTERFACE_MAGIC);

	sensor_peri = container_of(itf, struct fimc_is_device_sensor_peri, sensor_interface);
	BUG_ON(!sensor_peri);

	if (!test_bit(FIMC_IS_SENSOR_ACTUATOR_AVAILABLE, &sensor_peri->peri_state)) {
		dbg_sen_itf("%s: FIMC_IS_SENSOR_ACTUATOR_NOT_AVAILABLE\n", __func__);
		*position = 0;
		goto p_err;
	}

	actuator_itf = &itf->actuator_itf;
	BUG_ON(!actuator_itf);

	*position = actuator_itf->virtual_pos;

p_err:

	return ret;
}

int get_prev_frame_position(struct fimc_is_sensor_interface *itf,
		u32 *position, u32 frame_diff)
{
	int ret = 0;
	struct fimc_is_device_sensor_peri *sensor_peri = NULL;
	u32 cur_frame_cnt, prev_frame_cnt, index;

	BUG_ON(!itf);
	BUG_ON(!position);
	BUG_ON(itf->magic != SENSOR_INTERFACE_MAGIC);

	sensor_peri = container_of(itf, struct fimc_is_device_sensor_peri, sensor_interface);
	BUG_ON(!sensor_peri);

	if (!test_bit(FIMC_IS_SENSOR_ACTUATOR_AVAILABLE, &sensor_peri->peri_state)) {
		dbg_sen_itf("%s: FIMC_IS_SENSOR_ACTUATOR_NOT_AVAILABLE\n", __func__);
		*position = 0;
		goto p_err;
	}

	cur_frame_cnt = get_frame_count(itf);
	prev_frame_cnt = cur_frame_cnt - frame_diff;
	index = (cur_frame_cnt - frame_diff) % EXPECT_DM_NUM;
	*position = sensor_peri->cis.expecting_lens_udm[index].pos;

	dbg_sen_itf("%s: cur_frame(%d), prev_frame(%d), prev_position(%d)\n",
			__func__, cur_frame_cnt, prev_frame_cnt, *position);

p_err:

	return ret;
}

int get_status(struct fimc_is_sensor_interface *itf, u32 *status)
{
	int ret = 0;
	struct fimc_is_device_sensor_peri *sensor_peri = NULL;

	if (unlikely(!itf)) {
		err("%s, interface in is NULL", __func__);
		ret = -EINVAL;
		goto p_err;
	}

	BUG_ON(!status);
	BUG_ON(itf->magic != SENSOR_INTERFACE_MAGIC);

	sensor_peri = container_of(itf, struct fimc_is_device_sensor_peri, sensor_interface);
	BUG_ON(!sensor_peri);

	if (!test_bit(FIMC_IS_SENSOR_ACTUATOR_AVAILABLE, &sensor_peri->peri_state)) {
		dbg_sen_itf("%s: FIMC_IS_SENSOR_ACTUATOR_NOT_AVAILABLE\n", __func__);
		*status = ACTUATOR_STATUS_NO_BUSY;
		goto p_err;
	}

	ret = sensor_get_ctrl(itf, V4L2_CID_ACTUATOR_GET_STATUS, status);
	if (ret < 0) {
		err("Actuator get status fail. ret(%d), return_value(%d)", ret, *status);
		ret = -EINVAL;
		goto p_err;
	}
p_err:
	return ret;
}
