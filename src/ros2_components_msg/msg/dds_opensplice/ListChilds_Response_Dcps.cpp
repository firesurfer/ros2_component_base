//******************************************************************
// 
//  Generated by IDL to C++ Translator
//  
//  File name: ListChilds_Response_Dcps.cpp
//  Source: /home/firesurfer/workspace/kairo3_ws/src/kairo3/ros2_components/build/build/ros2_components/rosidl_typesupport_opensplice_cpp/ros2_components_msg/srv/dds_opensplice/ListChilds_Response_Dcps.idl
//  Generated: Fri Jan 29 14:40:04 2016
//  OpenSplice V6.4.140407OSS
//  
//******************************************************************

#include "ListChilds_Response_Dcps.h"

#if DDS_USE_EXPLICIT_TEMPLATES
template class DDS_DCPSUVLSeq < ros2_components_msg::srv::dds_::ListChilds_Response_, struct ListChilds_Response_Seq_uniq_>;
#endif

const char * ros2_components_msg::srv::dds_::ListChilds_Response_TypeSupportInterface::_local_id = "IDL:ros2_components_msg/srv/dds_/ListChilds_Response_TypeSupportInterface:1.0";

ros2_components_msg::srv::dds_::ListChilds_Response_TypeSupportInterface_ptr ros2_components_msg::srv::dds_::ListChilds_Response_TypeSupportInterface::_duplicate (ros2_components_msg::srv::dds_::ListChilds_Response_TypeSupportInterface_ptr p)
{
   if (p) p->m_count++;
   return p;
}

DDS::Boolean ros2_components_msg::srv::dds_::ListChilds_Response_TypeSupportInterface::_local_is_a (const char * _id)
{
   if (strcmp (_id, ros2_components_msg::srv::dds_::ListChilds_Response_TypeSupportInterface::_local_id) == 0)
   {
      return true;
   }

   typedef DDS::TypeSupport NestedBase_1;

   if (NestedBase_1::_local_is_a (_id))
   {
      return true;
   }

   return false;
}

ros2_components_msg::srv::dds_::ListChilds_Response_TypeSupportInterface_ptr ros2_components_msg::srv::dds_::ListChilds_Response_TypeSupportInterface::_narrow (DDS::Object_ptr p)
{
   ros2_components_msg::srv::dds_::ListChilds_Response_TypeSupportInterface_ptr result = NULL;
   if (p && p->_is_a (ros2_components_msg::srv::dds_::ListChilds_Response_TypeSupportInterface::_local_id))
   {
      result = dynamic_cast < ros2_components_msg::srv::dds_::ListChilds_Response_TypeSupportInterface_ptr> (p);
      if (result) result->m_count++;
   }
   return result;
}

ros2_components_msg::srv::dds_::ListChilds_Response_TypeSupportInterface_ptr ros2_components_msg::srv::dds_::ListChilds_Response_TypeSupportInterface::_unchecked_narrow (DDS::Object_ptr p)
{
   ros2_components_msg::srv::dds_::ListChilds_Response_TypeSupportInterface_ptr result;
   result = dynamic_cast < ros2_components_msg::srv::dds_::ListChilds_Response_TypeSupportInterface_ptr> (p);
   if (result) result->m_count++;
   return result;
}

const char * ros2_components_msg::srv::dds_::ListChilds_Response_DataWriter::_local_id = "IDL:ros2_components_msg/srv/dds_/ListChilds_Response_DataWriter:1.0";

ros2_components_msg::srv::dds_::ListChilds_Response_DataWriter_ptr ros2_components_msg::srv::dds_::ListChilds_Response_DataWriter::_duplicate (ros2_components_msg::srv::dds_::ListChilds_Response_DataWriter_ptr p)
{
   if (p) p->m_count++;
   return p;
}

DDS::Boolean ros2_components_msg::srv::dds_::ListChilds_Response_DataWriter::_local_is_a (const char * _id)
{
   if (strcmp (_id, ros2_components_msg::srv::dds_::ListChilds_Response_DataWriter::_local_id) == 0)
   {
      return true;
   }

   typedef DDS::DataWriter NestedBase_1;

   if (NestedBase_1::_local_is_a (_id))
   {
      return true;
   }

   return false;
}

ros2_components_msg::srv::dds_::ListChilds_Response_DataWriter_ptr ros2_components_msg::srv::dds_::ListChilds_Response_DataWriter::_narrow (DDS::Object_ptr p)
{
   ros2_components_msg::srv::dds_::ListChilds_Response_DataWriter_ptr result = NULL;
   if (p && p->_is_a (ros2_components_msg::srv::dds_::ListChilds_Response_DataWriter::_local_id))
   {
      result = dynamic_cast < ros2_components_msg::srv::dds_::ListChilds_Response_DataWriter_ptr> (p);
      if (result) result->m_count++;
   }
   return result;
}

ros2_components_msg::srv::dds_::ListChilds_Response_DataWriter_ptr ros2_components_msg::srv::dds_::ListChilds_Response_DataWriter::_unchecked_narrow (DDS::Object_ptr p)
{
   ros2_components_msg::srv::dds_::ListChilds_Response_DataWriter_ptr result;
   result = dynamic_cast < ros2_components_msg::srv::dds_::ListChilds_Response_DataWriter_ptr> (p);
   if (result) result->m_count++;
   return result;
}

const char * ros2_components_msg::srv::dds_::ListChilds_Response_DataReader::_local_id = "IDL:ros2_components_msg/srv/dds_/ListChilds_Response_DataReader:1.0";

ros2_components_msg::srv::dds_::ListChilds_Response_DataReader_ptr ros2_components_msg::srv::dds_::ListChilds_Response_DataReader::_duplicate (ros2_components_msg::srv::dds_::ListChilds_Response_DataReader_ptr p)
{
   if (p) p->m_count++;
   return p;
}

DDS::Boolean ros2_components_msg::srv::dds_::ListChilds_Response_DataReader::_local_is_a (const char * _id)
{
   if (strcmp (_id, ros2_components_msg::srv::dds_::ListChilds_Response_DataReader::_local_id) == 0)
   {
      return true;
   }

   typedef DDS::DataReader NestedBase_1;

   if (NestedBase_1::_local_is_a (_id))
   {
      return true;
   }

   return false;
}

ros2_components_msg::srv::dds_::ListChilds_Response_DataReader_ptr ros2_components_msg::srv::dds_::ListChilds_Response_DataReader::_narrow (DDS::Object_ptr p)
{
   ros2_components_msg::srv::dds_::ListChilds_Response_DataReader_ptr result = NULL;
   if (p && p->_is_a (ros2_components_msg::srv::dds_::ListChilds_Response_DataReader::_local_id))
   {
      result = dynamic_cast < ros2_components_msg::srv::dds_::ListChilds_Response_DataReader_ptr> (p);
      if (result) result->m_count++;
   }
   return result;
}

ros2_components_msg::srv::dds_::ListChilds_Response_DataReader_ptr ros2_components_msg::srv::dds_::ListChilds_Response_DataReader::_unchecked_narrow (DDS::Object_ptr p)
{
   ros2_components_msg::srv::dds_::ListChilds_Response_DataReader_ptr result;
   result = dynamic_cast < ros2_components_msg::srv::dds_::ListChilds_Response_DataReader_ptr> (p);
   if (result) result->m_count++;
   return result;
}

const char * ros2_components_msg::srv::dds_::ListChilds_Response_DataReaderView::_local_id = "IDL:ros2_components_msg/srv/dds_/ListChilds_Response_DataReaderView:1.0";

ros2_components_msg::srv::dds_::ListChilds_Response_DataReaderView_ptr ros2_components_msg::srv::dds_::ListChilds_Response_DataReaderView::_duplicate (ros2_components_msg::srv::dds_::ListChilds_Response_DataReaderView_ptr p)
{
   if (p) p->m_count++;
   return p;
}

DDS::Boolean ros2_components_msg::srv::dds_::ListChilds_Response_DataReaderView::_local_is_a (const char * _id)
{
   if (strcmp (_id, ros2_components_msg::srv::dds_::ListChilds_Response_DataReaderView::_local_id) == 0)
   {
      return true;
   }

   typedef DDS::DataReaderView NestedBase_1;

   if (NestedBase_1::_local_is_a (_id))
   {
      return true;
   }

   return false;
}

ros2_components_msg::srv::dds_::ListChilds_Response_DataReaderView_ptr ros2_components_msg::srv::dds_::ListChilds_Response_DataReaderView::_narrow (DDS::Object_ptr p)
{
   ros2_components_msg::srv::dds_::ListChilds_Response_DataReaderView_ptr result = NULL;
   if (p && p->_is_a (ros2_components_msg::srv::dds_::ListChilds_Response_DataReaderView::_local_id))
   {
      result = dynamic_cast < ros2_components_msg::srv::dds_::ListChilds_Response_DataReaderView_ptr> (p);
      if (result) result->m_count++;
   }
   return result;
}

ros2_components_msg::srv::dds_::ListChilds_Response_DataReaderView_ptr ros2_components_msg::srv::dds_::ListChilds_Response_DataReaderView::_unchecked_narrow (DDS::Object_ptr p)
{
   ros2_components_msg::srv::dds_::ListChilds_Response_DataReaderView_ptr result;
   result = dynamic_cast < ros2_components_msg::srv::dds_::ListChilds_Response_DataReaderView_ptr> (p);
   if (result) result->m_count++;
   return result;
}



