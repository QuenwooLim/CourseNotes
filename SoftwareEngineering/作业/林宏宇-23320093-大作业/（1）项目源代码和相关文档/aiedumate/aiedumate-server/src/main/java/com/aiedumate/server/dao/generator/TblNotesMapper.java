package com.aiedumate.server.dao.generator;

import com.aiedumate.server.entity.TblNotes;
import com.aiedumate.server.entity.TblNotesExample;
import java.util.List;
import org.apache.ibatis.annotations.Param;

public interface TblNotesMapper {
    long countByExample(TblNotesExample example);

    int deleteByExample(TblNotesExample example);

    int deleteByPrimaryKey(Integer id);

    int insert(TblNotes row);

    int insertSelective(TblNotes row);

    List<TblNotes> selectByExampleWithBLOBs(TblNotesExample example);

    List<TblNotes> selectByExample(TblNotesExample example);

    TblNotes selectByPrimaryKey(Integer id);

    int updateByExampleSelective(@Param("row") TblNotes row, @Param("example") TblNotesExample example);

    int updateByExampleWithBLOBs(@Param("row") TblNotes row, @Param("example") TblNotesExample example);

    int updateByExample(@Param("row") TblNotes row, @Param("example") TblNotesExample example);

    int updateByPrimaryKeySelective(TblNotes row);

    int updateByPrimaryKeyWithBLOBs(TblNotes row);

    int updateByPrimaryKey(TblNotes row);
}