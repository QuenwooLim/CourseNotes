package com.aiedumate.client.dao;

import com.aiedumate.client.dao.generator.TblNotesMapper;
import org.apache.ibatis.annotations.Mapper;
import org.apache.ibatis.annotations.Param;
import org.apache.ibatis.annotations.Select;
import org.apache.ibatis.annotations.Update;

import java.util.List;

@Mapper
public interface NotesMapper extends TblNotesMapper {
    @Select({
            "select course from tbl_notes",
            "where username = #{username}",
            "group by course"
    })
    List<String> getAllCourse(@Param("username") String username);
}
