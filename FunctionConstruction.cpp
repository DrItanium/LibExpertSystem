//Copyright (c) 2012, Joshua Scoggins 
//All rights reserved.
//
//Redistribution and use in source and binary forms, with or without
//modification, are permitted provided that the following conditions are met:
//    * Redistributions of source code must retain the above copyright
//      notice, this list of conditions and the following disclaimer.
//    * Redistributions in binary form must reproduce the above copyright
//      notice, this list of conditions and the following disclaimer in the
//      documentation and/or other materials provided with the distribution.
//    * Neither the name of Joshua Scoggins nor the
//      names of its contributors may be used to endorse or promote products
//      derived from this software without specific prior written permission.
//
//THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
//ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
//WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
//DISCLAIMED. IN NO EVENT SHALL Joshua Scoggins BE LIABLE FOR ANY
//DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
//(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
//LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
//ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
//(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
//SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include "FunctionConstruction.h"
void CLIPSFunctionBuilder::addFields(Function* func) {
   const char* nm = fn.getName().data();
   name = std::string(nm);
   addFields((GlobalValue*)func, "");
   if(func->isVarArg()) addTrueField("IsVarArg");
   if(func->isIntrinsic()) addTrueField("IsIntrinsic");
   if(func->hasGC()) addTrueField("HasGC");
   if(func->doesNotAccessMemory()) addTrueField("DoesNotAccessMemory");
   if(func->onlyReadsMemory()) addTrueField("OnlyReadsMemory");
   if(func->doesNotReturn()) addTrueField("DoesNotReturn");
   if(func->doesNotThrow()) addTrueField("DoesNotThrow");
   if(func->hasUWTable()) addTrueField("HasUWTable");
   if(func->needsUnwindTableEntry()) addTrueField("NeedsUnwindTableEntry");
   if(func->hasStructRetAttr()) addTrueField("HasStructRetAttr");
   //doesNotAlias and doesNotCapture need to be handled somehow  
   FunctionNamer& namer = getNamer();
   addField("ReturnType", Route(func->getReturnType(), namer));
   //if we don't take anything in then ignore the creation process
   Function::arg_iterator s = func->arg_begin();
   Function::arg_iterator f = func->arg_end();
   if(s != f) {
      openField("Arguments");
      for(; s != f; ++s) {
         Argument* a = s;
         appendValue(Route(a, (char*)nm, namer));
      }
      closeField("Arguments");
   }
}

