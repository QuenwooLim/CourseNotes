package com.aiedumate.client.dao.generator;

import com.aiedumate.client.entity.TblUserSessions;
import com.aiedumate.client.entity.TblUserSessionsExample;
import java.util.List;
import org.apache.ibatis.annotations.Param;

public interface TblUserSessionsMapper {
    long countByExample(TblUserSessionsExample example);

    int deleteByExample(TblUserSessionsExample example);

    int deleteByPrimaryKey(String sessionId);

    int insert(TblUserSessions row);

    int insertSelective(TblUserSessions row);

    List<TblUserSessions> selectByExampleWithBLOBs(TblUserSessionsExample example);

    List<TblUserSessions> selectByExample(TblUserSessionsExample example);

    TblUserSessions selectByPrimaryKey(String sessionId);

    int updateByExampleSelective(@Param("row") TblUserSessions row, @Param("example") TblUserSessionsExample example);

    int updateByExampleWithBLOBs(@Param("row") TblUserSessions row, @Param("example") TblUserSessionsExample example);

    int updateByExample(@Param("row") TblUserSessions row, @Param("example") TblUserSessionsExample example);

    int updateByPrimaryKeySelective(TblUserSessions row);

    int updateByPrimaryKeyWithBLOBs(TblUserSessions row);

    int updateByPrimaryKey(TblUserSessions row);
}