/*
 * Copyright (c) 2011-2015, Intel Corporation
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation and/or
 * other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its contributors
 * may be used to endorse or promote products derived from this software without
 * specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#include "criterion/Criteria.h"
#include "criterion/InclusiveCriterion.h"

#include <stdexcept>

namespace core
{
namespace criterion
{

Criteria::Criteria() : mCriteria()
{
}

Criterion* Criteria::getCriterionPointer(const std::string& name) const
{
    // Bound exception aware code to criteria, others Pfw parts will check nullptr.
    try {
        return mCriteria.at(name).get();
    }
    catch (std::out_of_range&) {
        return nullptr;
    }
}

Criterion* Criteria::createExclusiveCriterion(const std::string& name, core::log::Logger& logger)
{
    mCriteria.emplace(name, CriterionWrapper(new Criterion(name, logger)));
    return getCriterionPointer(name);
}

Criterion* Criteria::createInclusiveCriterion(const std::string& name, core::log::Logger& logger)
{
    mCriteria.emplace(name, CriterionWrapper(new InclusiveCriterion(name, logger)));
    return getCriterionPointer(name);
}

Criterion* Criteria::getSelectionCriterion(const std::string& name)
{
    return getCriterionPointer(name);
}

const Criterion* Criteria::getSelectionCriterion(const std::string& name) const
{
    return getCriterionPointer(name);
}

void Criteria::listSelectionCriteria(std::list<std::string>& results,
                                     bool withTypeInfo,
                                     bool humanReadable) const
{
    for (auto& criterion : mCriteria) {
        results.push_back(criterion.second->getFormattedDescription(withTypeInfo, humanReadable));
    }
}

void Criteria::resetModifiedStatus()
{
    for (auto& criterion : mCriteria) {
        criterion.second->resetModifiedStatus();
    }
}

void Criteria::toXml(CXmlElement& xmlElement,
                     CXmlSerializingContext& serializingContext) const
{
    for (auto& criterion : mCriteria) {

        CXmlElement xmlChildElement;
        xmlElement.createChild(xmlChildElement, "SelectionCriterion");
        criterion.second->toXml(xmlChildElement, serializingContext);
    }
}

} /** criterion namespace */
} /** core namespace */