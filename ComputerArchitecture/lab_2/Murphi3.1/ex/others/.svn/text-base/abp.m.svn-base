-------------------------------------------------------------------------
-- Copyright (C) 1992, 1993 by the Board of Trustees of 		 
-- Leland Stanford Junior University.					 
--									 
-- This description is provided to serve as an example of the use	 
-- of the Murphi description language and verifier, and as a benchmark	 
-- example for other verification efforts.				 
--									 
-- License to use, copy, modify, sell and/or distribute this description 
-- and its documentation any purpose is hereby granted without royalty,  
-- subject to the following terms and conditions, provided		 
--									 
-- appear in all copies of this description.				 
-- 									 
-- 2.  The Murphi group at Stanford University must be acknowledged	 
-- in any publication describing work that makes use of this example. 	 
-- 									 
-- Nobody vouches for the accuracy or usefulness of this description	 
-- for any purpose.							 

----------------------------------------------------------------------
-- Filename: 	abp.m
-- Content:	Simplex communication over a lossy channel
-- 		using a 1 bit sequence number
-- Version:	Murphi 2.3
-- Engineer:	Originally by C. Han Yang, 1991
--		Update to Murphi 2.3 by Ralph Melton, 1993.
----------------------------------------------------------------------
/*
Simplex communication over a lossy media using a 1 bit sequence number
*/
Type
  PacketType : Enum {EMPTY, PACKET, ACK, CORRUPT};
  ValueType : 0..1;
  PacketRecord : Record
                   Status : PacketType;
                   Value : ValueType;
                   SeqNo : ValueType
                 End;
Var
  X2RChannel : PacketRecord;  -- Channel from Transmitter to Receiver
  R2XChannel : PacketRecord;  -- Channel from Receiver to Transmitter
  Value : ValueType;    -- Value to be sent by the Transmitter
  OldValue : ValueType; -- Value sent by the Transmitter in last message
  SentOne : 0..1;       -- Flage indicating that a "1" has been sent
  ReceivedValue : ValueType;  -- Inclusive OR of values received by
			      -- Receiver
  XSeqNo : ValueType;    -- Sequence number sent by the transmitter
  RSeqNo : ValueType;    -- Sequence number next expected by the Receiver

Procedure SendPacket (Var Channel : PacketRecord; Status : PacketType;
	 	      Value, SeqNo : ValueType);
Begin
  Channel.Status := Status;
  Channel.Value := Value;
  Channel.SeqNo := SeqNo;
End;

Rule "Generate Value 0 to send"
  R2XChannel.Status != EMPTY
==>
Begin
  Value := 0;
End;

Rule "Generate Value 1 to send"
  (R2XChannel.Status != EMPTY) & (SentOne = 0)
==>
Begin
  Value := 1;
End;

Rule "Transmitter"
  R2XChannel.Status != EMPTY
==>
Begin
  Switch R2XChannel.Status
    Case CORRUPT:
      -- Acknowledgement from Receiver has been corrupted, Send old Value
      SendPacket (X2RChannel, PACKET, OldValue, XSeqNo);
    Case ACK:
      -- 
      If (R2XChannel.SeqNo != XSeqNo)
      Then
        -- Last message was properly accepted.  Send new packet
        XSeqNo := (XSeqNo = 1) ? 0 : 1;
        If(SentOne = 1)
        Then
          Value := 0;
        End;
        OldValue := Value;
        SendPacket (X2RChannel, PACKET, Value, XSeqNo);
        If (Value = 1)
        Then
          SentOne := 1;
        End;
      Else
        -- Last message was corrupted.  Must resend data.
        SendPacket (X2RChannel, PACKET, OldValue, XSeqNo);
      End;
    Else
      Error "Invalid Packet Type received at the Transmitter\n";
  End;
  Clear R2XChannel;
End;

Rule "Receiver"
  X2RChannel.Status != EMPTY
==>
Begin
  Switch X2RChannel.Status
    Case CORRUPT:
      -- Send acknowledgement without updating the sequence number
      SendPacket (R2XChannel, ACK, 0, RSeqNo);
    Case PACKET:
      If (X2RChannel.SeqNo = RSeqNo)
      Then
        -- Packet is accepted
        RSeqNo := (RSeqNo = 1) ? 0 : 1;
        SendPacket (R2XChannel, ACK, 0, RSeqNo);

        -- Check for duplicated packet
        If ( (X2RChannel.Value = 1) & (ReceivedValue = 1) )
        Then
          Error "Receiver has received 2 packets with value 1\n";
        End;
        If (X2RChannel.Value = 1)
        Then
          ReceivedValue := 1;
        End;
      Else
        SendPacket (R2XChannel, ACK, 0, RSeqNo);
      End;
    Else
      Error "Invalid Packet Type received at the Receiver\n";
  End;
  Clear X2RChannel;
End;

/*
The following 2 rules nondeterministically corrupts the channels
*/

Rule "Corrupt X2RChannel"
  (X2RChannel.Status != EMPTY) & (X2RChannel.Status != CORRUPT)
==>
Begin
  Clear X2RChannel;
  X2RChannel.Status := CORRUPT;
End;

Rule "Corrupt R2XChannel"
  (R2XChannel.Status != EMPTY) & (R2XChannel.Status != CORRUPT)
==>
Begin
  Clear R2XChannel;
  R2XChannel.Status := CORRUPT;
End;

Startstate
Begin
  Value := 0;
  OldValue := 0;
  SentOne := 0;
  XSeqNo := 1;
  RSeqNo := 0;
  Clear X2RChannel;
  Clear R2XChannel;
  R2XChannel.Status := ACK;
  R2XChannel.Value := 0;
  R2XChannel.SeqNo := 0;
  ReceivedValue := 0;
End; --Startstate;



