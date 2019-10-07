#pragma once

#include <materials/CIMaterial.h>
#include <core/CTexture.h>

namespace engine
{

    class CLambertMaterial : public CIMaterial
    {

    public :

        CVec3 ambient;
        CVec3 diffuse;

        CLambertMaterial( const std::string& name,
                          const CVec3& ambientColor,
                          const CVec3& diffuseColor,
                          CTexture* diffuseMap = nullptr );

        ~CLambertMaterial();

        void setDiffuseMap( CTexture* diffuseMap ) { m_diffuseMap = diffuseMap; }

        void bind( CShader* shaderPtr ) override;
        void unbind() override;

        CTexture* diffuseMap() const { return m_diffuseMap; }

    protected :

        std::string _toStringInternal() override;

        CTexture* m_diffuseMap;

    };

}
