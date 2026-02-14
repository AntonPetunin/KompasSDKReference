cmake_minimum_required(VERSION 3.20.0 FATAL_ERROR)

project(Steps C CXX)

#Step1
add_subdirectory(Step1)
set_target_properties(Step1 PROPERTIES FOLDER "Steps")
#Step2
add_subdirectory(Step2)
set_target_properties(Step2 PROPERTIES FOLDER "Steps")
#Step3
add_subdirectory(Step3)
set_target_properties(Step3 PROPERTIES FOLDER "Steps")
#Step7
add_subdirectory(Step7)
set_target_properties(Step7 PROPERTIES FOLDER "Steps")

#Step1_KsAPI_2D
add_subdirectory(Step1_KsAPI_2D)
set_target_properties(Step1_KsAPI_2D PROPERTIES FOLDER "Steps")
#Step2_KsAPI_2D
add_subdirectory(Step2_KsAPI_2D)
set_target_properties(Step2_KsAPI_2D PROPERTIES FOLDER "Steps")
#Step2_KsAPI_3D
add_subdirectory(Step2_KsAPI_3D)
set_target_properties(Step2_KsAPI_3D PROPERTIES FOLDER "Steps")
#Step3_KsAPI_2D
add_subdirectory(Step3_KsAPI_2D)
set_target_properties(Step3_KsAPI_2D PROPERTIES FOLDER "Steps")
#Step3_KsAPI_3D
add_subdirectory(Step3_KsAPI_3D)
set_target_properties(Step3_KsAPI_3D PROPERTIES FOLDER "Steps")
#Step3a
add_subdirectory(Step3a)
set_target_properties(Step3a PROPERTIES FOLDER "Steps")

#Step3D1
add_subdirectory(Step3D1)
set_target_properties(Step3D1 PROPERTIES FOLDER "Steps")
#Step3D2
add_subdirectory(Step3D2)
set_target_properties(Step3D2 PROPERTIES FOLDER "Steps")
#Step3D3
add_subdirectory(Step3D3)
set_target_properties(Step3D3 PROPERTIES FOLDER "Steps")

#Step4
add_subdirectory(Step4)
set_target_properties(Step4 PROPERTIES FOLDER "Steps")
#Step4_KsAPI_2D
add_subdirectory(Step4_KsAPI_2D)
set_target_properties(Step4_KsAPI_2D PROPERTIES FOLDER "Steps")
#Step4_KsAPI_3D
add_subdirectory(Step4_KsAPI_3D)
set_target_properties(Step4_KsAPI_3D PROPERTIES FOLDER "Steps")
#Step5
add_subdirectory(Step5)
set_target_properties(Step5 PROPERTIES FOLDER "Steps")
#Step8_KsAPI
add_subdirectory(Step8_KsAPI)
set_target_properties(Step8_KsAPI PROPERTIES FOLDER "Steps")
#Step9
add_subdirectory(Step9)
set_target_properties(Step9 PROPERTIES FOLDER "Steps")
#Step10
add_subdirectory(Step10)
set_target_properties(Step10 PROPERTIES FOLDER "Steps")
#Step12
add_subdirectory(Step12)
set_target_properties(Step12 PROPERTIES FOLDER "Steps")
#Studs3D1
add_subdirectory(Studs3D1)
set_target_properties(Studs3D1 PROPERTIES FOLDER "Steps")
