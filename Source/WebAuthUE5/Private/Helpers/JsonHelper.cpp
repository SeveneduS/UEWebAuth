// Fill out your copyright notice in the Description page of Project Settings.


#include "Helpers/JsonHelper.h"

template<typename StructType>
void JsonHelper::StructToJSON(StructType FilledStruct, FString& StringOutput)
{
	FJsonObjectConverter::UStructToJsonObjectString(StructType::StaticStruct(),
		&FilledStruct,
		StringOutput,
		0, 0);
}

template<typename StructType>
void JsonHelper::StructFromJSON(FHttpResponsePtr Response, StructType& StructOutput)
{
	StructType StructData;
	FString JsonString = Response->GetContentAsString();
	FJsonObjectConverter::JsonObjectStringToUStruct<StructType>(JsonString,
		&StructOutput,
		0, 0);
}