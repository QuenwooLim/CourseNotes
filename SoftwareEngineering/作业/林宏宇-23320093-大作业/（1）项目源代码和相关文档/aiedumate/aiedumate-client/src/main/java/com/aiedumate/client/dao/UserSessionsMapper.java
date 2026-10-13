package com.aiedumate.client.dao;

import com.aiedumate.client.dao.generator.TblUserSessionsMapper;
import org.apache.ibatis.annotations.Mapper;
import org.apache.ibatis.annotations.Param;
import org.apache.ibatis.annotations.Update;

@Mapper
public interface UserSessionsMapper extends TblUserSessionsMapper {
    @Update({
            "update tbl_user_sessions set is_expired = true",
            "where username = #{username}"
    })
    int invalidateAllSessions(@Param("username") String username);
}
