package com.aiedumate.client.dao.generator;

import com.aiedumate.client.entity.TblUserConversations;
import com.aiedumate.client.entity.TblUserConversationsExample;
import java.util.List;
import org.apache.ibatis.annotations.Param;

public interface TblUserConversationsMapper {
    long countByExample(TblUserConversationsExample example);

    int deleteByExample(TblUserConversationsExample example);

    int deleteByPrimaryKey(Integer id);

    int insert(TblUserConversations row);

    int insertSelective(TblUserConversations row);

    List<TblUserConversations> selectByExampleWithBLOBs(TblUserConversationsExample example);

    List<TblUserConversations> selectByExample(TblUserConversationsExample example);

    TblUserConversations selectByPrimaryKey(Integer id);

    int updateByExampleSelective(@Param("row") TblUserConversations row, @Param("example") TblUserConversationsExample example);

    int updateByExampleWithBLOBs(@Param("row") TblUserConversations row, @Param("example") TblUserConversationsExample example);

    int updateByExample(@Param("row") TblUserConversations row, @Param("example") TblUserConversationsExample example);

    int updateByPrimaryKeySelective(TblUserConversations row);

    int updateByPrimaryKeyWithBLOBs(TblUserConversations row);

    int updateByPrimaryKey(TblUserConversations row);
}