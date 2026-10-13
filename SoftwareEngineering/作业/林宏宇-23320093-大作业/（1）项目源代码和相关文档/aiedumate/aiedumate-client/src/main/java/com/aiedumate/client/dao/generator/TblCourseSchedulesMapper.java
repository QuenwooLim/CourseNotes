package com.aiedumate.client.dao.generator;

import com.aiedumate.client.entity.TblCourseSchedules;
import com.aiedumate.client.entity.TblCourseSchedulesExample;
import java.util.List;
import org.apache.ibatis.annotations.Param;

public interface TblCourseSchedulesMapper {
    long countByExample(TblCourseSchedulesExample example);

    int deleteByExample(TblCourseSchedulesExample example);

    int deleteByPrimaryKey(Integer id);

    int insert(TblCourseSchedules row);

    int insertSelective(TblCourseSchedules row);

    List<TblCourseSchedules> selectByExample(TblCourseSchedulesExample example);

    TblCourseSchedules selectByPrimaryKey(Integer id);

    int updateByExampleSelective(@Param("row") TblCourseSchedules row, @Param("example") TblCourseSchedulesExample example);

    int updateByExample(@Param("row") TblCourseSchedules row, @Param("example") TblCourseSchedulesExample example);

    int updateByPrimaryKeySelective(TblCourseSchedules row);

    int updateByPrimaryKey(TblCourseSchedules row);
}