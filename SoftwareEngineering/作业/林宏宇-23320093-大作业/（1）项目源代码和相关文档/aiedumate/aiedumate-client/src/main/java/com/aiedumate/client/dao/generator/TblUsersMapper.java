package com.aiedumate.client.dao.generator;

import com.aiedumate.client.entity.TblUsers;
import com.aiedumate.client.entity.TblUsersExample;
import java.util.List;
import org.apache.ibatis.annotations.Param;

public interface TblUsersMapper {
    long countByExample(TblUsersExample example);

    int deleteByExample(TblUsersExample example);

    int deleteByPrimaryKey(String username);

    int insert(TblUsers row);

    int insertSelective(TblUsers row);

    List<TblUsers> selectByExample(TblUsersExample example);

    TblUsers selectByPrimaryKey(String username);

    int updateByExampleSelective(@Param("row") TblUsers row, @Param("example") TblUsersExample example);

    int updateByExample(@Param("row") TblUsers row, @Param("example") TblUsersExample example);

    int updateByPrimaryKeySelective(TblUsers row);

    int updateByPrimaryKey(TblUsers row);
}