#ifndef __MODEL_JSON_H__
#define __MODEL_JSON_H__

const char recognition_model_string_json[] = {"{\"NumModels\":1,\"ModelIndexes\":{\"0\":\"ILLEGAL_RANK_0\"},\"ModelDescriptions\":[{\"Name\":\"ILLEGAL_RANK_0\",\"ClassMaps\":{\"1\":\"ChainSaw\",\"2\":\"Humans\",\"3\":\"Normal\",\"0\":\"Unknown\"},\"ModelType\":\"PME\",\"FeatureFunctions\":[\"NegativeZeroCrossings\",\"MFCC\",\"MFCC\",\"MFCC\",\"MFCC\",\"DominantFrequency\",\"PowerSpectrum\",\"PowerSpectrum\",\"AbsoluteAreaofLowFrequency\"]}]}"};

int recognition_model_string_json_len = sizeof(recognition_model_string_json);

#endif /* __MODEL_JSON_H__ */
