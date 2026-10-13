package com.aiedumate.server.handler;

import com.aiedumate.server.entity.TblNotes;
import com.aiedumate.server.service.NotesService;
import lombok.extern.slf4j.Slf4j;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import java.util.List;

@Service
@Slf4j
public class NotesHandler {
    @Autowired
    private NotesService notesService;
    private String printNotes(TblNotes notes) {
        String result = String.format("username:%s,course:%s,title:%s,content:%s",
                notes.getUsername(),
                notes.getCourse(),
                notes.getTitle(),
                notes.getContent());

        return result;
    }

    public String createNotes(TblNotes notes) {
        log.info(printNotes(notes));
        int result = notesService.insert(notes);
        log.info("insert result:{}", result);

        String resultText;
        if(result == 1){
            resultText = "添加笔记成功！";
        } else {
            resultText = "添加笔记失败，请重试！";
        }
        return resultText;
    }

    public String deleteNotes(TblNotes notes) {
        log.info(printNotes(notes));
        int result = notesService.delete(notes);
        log.info("delete result:{}", result);

        String resultText;
        if(result >= 0){
            resultText = "删除笔记成功，数量：" +  result;
        } else {
            resultText = "删除笔记失败，请重试！";
        }
        return resultText;
    }

    public String queryNotes(TblNotes notes) {
        log.info(printNotes(notes));
        List<TblNotes> notesList = notesService.query(notes);
        log.info("query result:{}", notesList.size());

        StringBuilder  resultText = new StringBuilder();
        for(TblNotes entity : notesList) {
            String line = String.format("课程：%s, 笔记标题: %s, 笔记内容：%s\\n"
                    , entity.getCourse()
                    , entity.getTitle()
                    , entity.getContent());
            resultText.append(line);
        }

        return resultText.toString();
    }
}
