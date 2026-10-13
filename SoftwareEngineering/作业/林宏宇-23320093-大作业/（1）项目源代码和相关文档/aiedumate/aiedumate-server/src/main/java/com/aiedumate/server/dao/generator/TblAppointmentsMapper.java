package com.aiedumate.server.dao.generator;

import com.aiedumate.server.entity.TblAppointments;
import com.aiedumate.server.entity.TblAppointmentsExample;
import java.util.List;
import org.apache.ibatis.annotations.Param;

public interface TblAppointmentsMapper {
    long countByExample(TblAppointmentsExample example);

    int deleteByExample(TblAppointmentsExample example);

    int deleteByPrimaryKey(Integer id);

    int insert(TblAppointments row);

    int insertSelective(TblAppointments row);

    List<TblAppointments> selectByExampleWithBLOBs(TblAppointmentsExample example);

    List<TblAppointments> selectByExample(TblAppointmentsExample example);

    TblAppointments selectByPrimaryKey(Integer id);

    int updateByExampleSelective(@Param("row") TblAppointments row, @Param("example") TblAppointmentsExample example);

    int updateByExampleWithBLOBs(@Param("row") TblAppointments row, @Param("example") TblAppointmentsExample example);

    int updateByExample(@Param("row") TblAppointments row, @Param("example") TblAppointmentsExample example);

    int updateByPrimaryKeySelective(TblAppointments row);

    int updateByPrimaryKeyWithBLOBs(TblAppointments row);

    int updateByPrimaryKey(TblAppointments row);
}